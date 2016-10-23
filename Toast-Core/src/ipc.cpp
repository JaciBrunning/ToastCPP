#include "toast/net/socket.hpp"
#include "toast/ipc.hpp"
#include "toast/logger.hpp"

#include <thread>
#include <map>
#include <regex>

using namespace Toast;

static bool started = false;
static Logger _log("Toast-IPC");
static Net::Socket::SOCKET sock;
static std::thread read_thread;
static std::vector<std::tuple<std::string, IPC::MessageListener, void *, bool> > listeners;

static Net::Socket::SocketAddress temp_addr("127.0.0.1", 6300);
static Net::Socket::SocketAddress temp_addr_recv;
static int my_module_idx = 0;

static char message[4096];

static void read_thread_func() {
	char msg[4096];
#ifdef OS_WIN
	int *ptr = temp_addr_recv.raw_address_len_ptr();
#else
	unsigned int *ptr = (unsigned int *)temp_addr_recv.raw_address_len_ptr();
#endif
	while (1) {
		int len = ::recvfrom(sock, &msg[0], 4096, 0, (struct sockaddr *)temp_addr_recv.raw_address(), ptr);
		int source_module = (int8_t)msg[0];
		int handle_len = (uint8_t)msg[1];
		char *handle_chars = &msg[2];
		std::string handle(handle_chars, handle_len);

		if (len == -1) {
			_log.error("IPC Socket Receive Error: " + std::to_string(Net::Socket::socket_last_error()));
		}

		for (auto it = listeners.begin(); it != listeners.end(); ) {
			auto entry = *it;
			std::string s = std::get<0>(entry);
			IPC::MessageListener l = std::get<1>(entry);
			void *param = std::get<2>(entry);
			bool once = std::get<3>(entry);
			if (handle == s) {
				l(handle, (void *)&msg[2 + handle_len], len - handle_len - 2, source_module, param);	// Call the listener
				if (once) {
					it = listeners.erase(it);
				} else {
					++it;
				}
			}
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

void IPC::sendto(std::string handle, void *data, int data_length, int module_idx) {
	temp_addr.set_port(6300 + module_idx + 1);
	int total_length = handle.length() + data_length + 2;
	message[0] = (int8_t)(my_module_idx);
	message[1] = (uint8_t)(handle.length());
	memcpy(message + 2, handle.c_str(), handle.length());
	memcpy(message + 2 + handle.length(), data, data_length);
	int ret = ::sendto(sock, message, total_length, 0, (struct sockaddr *)temp_addr.raw_address(), temp_addr.raw_address_length());
	if (ret == -1) {
		_log.error("IPC Socket Send Error: " + std::to_string(Net::Socket::socket_last_error()));
	}
}

void IPC::listen(std::string handle, IPC::MessageListener listener, void *param) {
	listeners.push_back(std::make_tuple(handle, listener, param, false));
}

void IPC::listen_once(std::string handle, IPC::MessageListener listener, void *param) {
	listeners.push_back(std::make_tuple(handle, listener, param, true));
}