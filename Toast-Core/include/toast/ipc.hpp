#pragma once

#include "toast/environment.hpp"
#include "toast/module.hpp"

namespace Toast {
	namespace IPC {
		typedef void (*MessageListener)(std::string handle, char *data, int data_length, int module_id);
		API void start(int module_id);
		API void send(std::string handle, const char *data, int data_length);
		API void broadcast(std::string handle, const char *data, int data_length);
		API void sendto(std::string handle, const char *data, int data_length, int module_id);
		API void listen(std::string handle, MessageListener listener);
	}
}