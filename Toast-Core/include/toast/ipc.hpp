#pragma once

#include "toast/environment.hpp"
#include "toast/module.hpp"

#include <functional>

namespace Toast {
	namespace IPC {
		typedef std::function<void(std::string handle, void *data, int data_length, int module_id, void *param)> MessageListener;
		API void start(int module_id);
		API void send(std::string handle, void *data, int data_length);
		API void broadcast(std::string handle, void *data, int data_length, bool bootstrap=false);
		API void sendto(std::string handle, void *data, int data_length, int module_id);
		API long listen(std::string handle, MessageListener listener, void *param = NULL);
		API void stop_listening(long ptr);
	}
}