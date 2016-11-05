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

static Net::Socket::SocketAddress temp_addr("127.0.0.1", 6300);
static Net::Socket::SocketAddress temp_addr_recv;
static int my_module_idx = 0;

static char message[4096];
static Concurrent::Mutex mtx_send;
static uint8_t ack_id;
static Concurrent::ConditionVariable ack_cond;
static uint8_t ack_response_id = -1;
static int8_t ack_response_module = -2;	// Set this to -2 so the first wait doesn't default to true for bootstrap messages

static void read_thread_func() {
	char msg[4096];
	char ack_msg[10];
#ifdef OS_WIN
	int *ptr = temp_addr_recv.raw_address_len_ptr();
#else
	unsigned int *ptr = (unsigned int *)temp_addr_recv.raw_address_len_ptr();
#endif
	while (1) {
		int len = ::recvfrom(sock, &msg[0], 4096, 0, (struct sockaddr *)temp_addr_recv.raw_address(), ptr);
		int source_module = (int8_t)msg[0];
		int handle_len = (uint8_t)msg[1];
		bool ack = (uint8_t)(msg[2]) != 0;
		char *handle_chars = &msg[3];
		std::string handle(handle_chars, handle_len);

		if (len == -1) {
			_log.error("IPC Socket Receive Error: " + std::to_string(Net::Socket::socket_last_error()));
		}

		if (handle == IPC::HANDLE_ACK) {
			ack_response_module = source_module;
			ack_response_id = msg[2];
			ack_cond.signal_all();
			continue;
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
				l(handle, (void *)&msg[3 + handle_len], len - handle_len - 2, source_module, param);	// Call the listener
			}
		}

		if (ack) {
			mtx_send.lock();
			temp_addr.set_port(6300 + source_module + 1);
			ack_msg[0] = my_module_idx;
			ack_msg[1] = IPC::HANDLE_ACK.length();
			ack_msg[2] = msg[2];
			memcpy(ack_msg + 3, IPC::HANDLE_ACK.c_str(), IPC::HANDLE_ACK.length());
			int ret = ::sendto(sock, ack_msg, 3 + IPC::HANDLE_ACK.length(), 0, (struct sockaddr *)temp_addr.raw_address(), temp_addr.raw_address_length());
			if (ret == -1) {
				_log.error("IPC Socket ACK Error: " + std::to_string(Net::Socket::socket_last_error()));
			}
			mtx_send.unlock();
		}
	}
}

void IPC::start(int module_id) {
	if (!started) {
		my_module_idx = module_id;
		sock = Net::Socket::socket_udp_create();

		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(6300 + module_id + 1);	// Bootstrap is module_id=-1, therefore port 6300
		if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
			_log.error("Could not bind UDP socket to localhost!");
			return;
		}

		read_thread = std::thread(read_thread_func);
		read_thread.detach();

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

// TODO Send w/ ack, use condition var

void IPC::sendto(std::string handle, void *data, int data_length, int module_idx) {
	mtx_send.lock();
	temp_addr.set_port(6300 + module_idx + 1);
	int total_length = handle.length() + data_length + 3;
	message[0] = (int8_t)(my_module_idx);
	message[1] = (uint8_t)(handle.length());
	message[2] = 0;
	memcpy(message + 3, handle.c_str(), handle.length());
	if (data_length > 0 && data != NULL)
		memcpy(message + 3 + handle.length(), data, data_length);
	int ret = ::sendto(sock, message, total_length, 0, (struct sockaddr *)temp_addr.raw_address(), temp_addr.raw_address_length());
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

	temp_addr.set_port(6300 + module_idx + 1);
	int total_length = handle.length() + data_length + 3;
	message[0] = (int8_t)(my_module_idx);
	message[1] = (uint8_t)(handle.length());
	message[2] = aid;
	memcpy(message + 3, handle.c_str(), handle.length());
	if (data_length > 0 && data != NULL)
		memcpy(message + 3 + handle.length(), data, data_length);
	int ret = ::sendto(sock, message, total_length, 0, (struct sockaddr *)temp_addr.raw_address(), temp_addr.raw_address_length());
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