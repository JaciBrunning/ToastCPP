#include "toast/net/socket.hpp"
#include "toast/ipc.hpp"
#include "toast/logger.hpp"

#include <thread>
#include <map>
#include <regex>
#include <queue>

#include "toast/crash.hpp"
#include "toast/concurrent/mutex.hpp"
#include "toast/concurrent/condition.hpp"

using namespace Toast;

static bool started = false;
static Logger _log("Toast-IPC");
static Net::Socket::SOCKET sock;
static std::thread read_thread;
static std::map<long, std::tuple<std::string, IPC::MessageListener, void *> > listeners;
static std::queue<long> remove_queue;
static long id = 0;

#ifdef OS_WIN
static Net::Socket::ServerSocket *serversock;
static Net::Socket::ClientSocket *clientsock;
#else
static Net::Socket::UnixDomainServerSocket *serversock;
static Net::Socket::UnixDomainClientSocket *clientsock;
#endif

static Net::Socket::SelectiveServerSocket *bootsock;
static Net::Socket::ClientSocket connected_sockets[128];

static int my_module_idx = 0;

static char message[4096];
static Concurrent::Mutex mtx_send;
static uint8_t ack_id;
static Concurrent::ConditionVariable ack_cond;
static uint8_t ack_response_id = -1;
static int8_t ack_response_module = -2;	// Set this to -2 so the first wait doesn't default to true for bootstrap messages

static char msg[4096];
static char ack_msg[10];

template <class READSOCKTYPE>
static void read_single(READSOCKTYPE *socket, int len) {
	if (len == -1) {
		_log.error("IPC Socket Receive Error: " + std::to_string(Net::Socket::socket_last_error()));
	}

	int source_module = (int8_t)msg[0];
	int target_module = (int8_t)msg[1];
	int handle_len = (uint8_t)msg[2];
	bool ack = (uint8_t)(msg[3]) != 0;
	char *handle_chars = &msg[4];

	std::string handle(handle_chars, handle_len);

	if (handle == IPC::HANDLE_CONNECT) {
		connected_sockets[source_module] = socket->get_socket();
		return;
	}

	if (my_module_idx == -1 && target_module != -1) {
		// This is meant to go to someone else
		connected_sockets[target_module].send(&msg[0], len);
		return;
	}

	if (handle == IPC::HANDLE_ACK) {
		ack_response_module = source_module;
		ack_response_id = msg[3];
		ack_cond.signal_all();
		return;
	}

	while (!remove_queue.empty()) {
		listeners.erase(remove_queue.front());
		remove_queue.pop();
	}

	for (auto it = listeners.begin(); it != listeners.end(); it++) {
		auto entry = *it;
		std::string s = std::get<0>(entry.second);
		IPC::MessageListener l = std::get<1>(entry.second);
		void *param = std::get<2>(entry.second);
		if (handle == s) {
			l(handle, (void *)&msg[4 + handle_len], len - handle_len - 2, source_module, param);	// Call the listener
		}
	}

	if (ack) {
		mtx_send.lock();
		ack_msg[0] = my_module_idx;
		ack_msg[1] = source_module;
		ack_msg[2] = IPC::HANDLE_ACK.length();
		ack_msg[3] = msg[3];
		memcpy(ack_msg + 4, IPC::HANDLE_ACK.c_str(), IPC::HANDLE_ACK.length());
		int ret = socket->send(ack_msg, 4 + IPC::HANDLE_ACK.length());
		if (ret == -1) {
			_log.error("IPC Socket ACK Error: " + std::to_string(Net::Socket::socket_last_error()));
		}
		mtx_send.unlock();
	}
}

void IPC::start(int module_id) {
	if (!started) {
		my_module_idx = module_id;

		if (module_id == -1) {
#ifdef OS_WIN
			serversock = new Net::Socket::ServerSocket(6300);
#else
			serversock = new Net::Socket::UnixDomainServerSocket("toast_ipc");
#endif
			bootsock = new Net::Socket::SelectiveServerSocket(serversock->get_socket(), 128);
			bootsock->prepare();
			int r = serversock->open();
			if (r == -1) {
				_log.error("IPC Server Bind Error: " + std::to_string(Net::Socket::socket_last_error()));
				return;
			}
			bootsock->on_data([](int client_id, Net::Socket::ClientSocket sock) {
				int ret = sock.read(&msg[0], 4096);
				if (ret == 0) {
					sock.close();
					return;
				}
				read_single(&sock, ret);
			});
			read_thread = std::thread([]() {
				while (bootsock->accept() >= 0) {};
			});
			read_thread.detach();
		} else {
#ifdef OS_WIN
			clientsock = new Net::Socket::ClientSocket("127.0.0.1", 6300);
#else
			clientsock = new Net::Socket::UnixDomainClientSocket("toast_ipc");
#endif
			_log.info("ClientSock");
			int r = clientsock->connect();
			if (r == -1) {
				_log.error("IPC Client Connection Error: " + std::to_string(Net::Socket::socket_last_error()));
				return;
			}
			read_thread = std::thread([]() {
				int ret = 0;
				while (1) {
					ret = clientsock->read(&msg[0], 4096);
					if (ret == 0) break;
					read_single(clientsock, ret);
				};
				_log.error("IPC Socket Client Stopped");
				clientsock->close();
			});
			// Send initial
			IPC::send(IPC::HANDLE_CONNECT, NULL, 0);
			read_thread.detach();
		}

		started = true;
	}
}

void IPC::send(std::string handle, void *data, int data_length) {
	IPC::sendto(handle, data, data_length, -1);
}

void IPC::broadcast(std::string handle, void *data, int data_length, bool bootstrap) {
	if (bootstrap) IPC::sendto(handle, data, data_length, -1);
	for (ModuleData mdata : get_all_modules()) {
		IPC::sendto(handle, data, data_length, mdata.module_idx);
	}
}

void IPC::sendto(std::string handle, void *data, int data_length, int module_idx) {
	mtx_send.lock();
	int total_length = handle.length() + data_length + 4;
	message[0] = (int8_t)(my_module_idx);
	message[1] = (int8_t)(module_idx);
	message[2] = (uint8_t)(handle.length());
	message[3] = 0;
	memcpy(message + 4, handle.c_str(), handle.length());
	if (data_length > 0 && data != NULL)
		memcpy(message + 4 + handle.length(), data, data_length);
	int ret;
	if (my_module_idx == -1) {
		ret = connected_sockets[module_idx].send(message, total_length);
	} else {
		ret = clientsock->send(message, total_length);
	}
	if (ret == -1) {
		_log.error("IPC Socket Send Error: " + std::to_string(Net::Socket::socket_last_error()));
	}
	mtx_send.unlock();
}

void IPC::send_with_ack(std::string handle, void *data, int data_length, int module_idx) {
	mtx_send.lock();

	// We can't have this be 0 otherwise we don't detect it as an ACK packet
	ack_id++;
	if (ack_id == 0) ack_id++;
	uint8_t aid = ack_id;

	int total_length = handle.length() + data_length + 4;
	message[0] = (int8_t)(my_module_idx);
	message[1] = (int8_t)(module_idx);
	message[2] = (uint8_t)(handle.length());
	message[3] = aid;
	memcpy(message + 4, handle.c_str(), handle.length());
	if (data_length > 0 && data != NULL)
		memcpy(message + 4 + handle.length(), data, data_length);
	int ret;
	if (my_module_idx == -1) {
		ret = connected_sockets[module_idx].send(message, total_length);
	} else {
		ret = clientsock->send(message, total_length);
	}
	if (ret == -1) {
		_log.error("IPC Socket Send Error: " + std::to_string(Net::Socket::socket_last_error()));
	}
	mtx_send.unlock();

	ack_cond.lock();
	while (ack_response_id != aid || ack_response_module != module_idx) {
		ack_cond.wait();
	}
	ack_cond.unlock();
}

long IPC::listen(std::string handle, IPC::MessageListener listener, void *param) {
	int i = id++;
	listeners[i] = std::make_tuple(handle, listener, param);
	return i;
}

void IPC::stop_listening(long id) {
	remove_queue.push(id);
}