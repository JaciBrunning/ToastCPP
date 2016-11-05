#include "io/io.hpp"
#include "toast/ipc.hpp"

void IO::update_now(bool wait) {
	if (wait)
		Toast::IPC::send_with_ack(IO::IPC_HANDLE_UPDATE, NULL, 0);
	else
		Toast::IPC::send(IO::IPC_HANDLE_UPDATE, NULL, 0);
}