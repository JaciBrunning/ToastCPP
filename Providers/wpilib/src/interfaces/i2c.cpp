#include "thp/wpi/interfaces.hpp"

#include "toast/ipc.hpp"
#include "toast/concurrent/mutex.hpp"
#include "toast/concurrent/condition.hpp"

#include "I2C.h"
#include "io/I2C.hpp"

#include <queue>
#include <vector>
#include <thread>

typedef struct {
	int message_idx, type;
	uint8_t *send_data;
	int send_size, recv_size;
	int reg_address;
	int module_id;
} I2C_OP;

typedef struct {
	IO::I2C::Port port;
	int addr, refcount;
	Toast::Concurrent::ConditionVariable cv;
	I2C *wpi_reference;
	bool marked_for_deletion = false;
	std::queue<I2C_OP> operations;
} I2C_INTERFACE;

static std::vector<I2C_INTERFACE *> interfaces;

static void _i2c_threadfunc(I2C_INTERFACE *intf) {
	char resp_buf[512];
	// This will break out of the loop if we've reached a reference count of 0
	while (!intf->marked_for_deletion) {
		intf->cv.lock();
		intf->cv.wait();
		if (!intf->marked_for_deletion && !intf->operations.empty()) {
			// We're not in queue for deletion, and we have stuff available
			I2C_OP op = intf->operations.front();
			IO::I2C_IPC::I2CResponseHeader header = { intf->port, intf->addr, /*length*/ op.recv_size, op.message_idx, /*success*/false };
			int off = sizeof(header);
			int total_size = off + header.length;
			memcpy(&resp_buf[0], &header, sizeof(header));

			if (op.type == 1) {
				// Transaction
				header.success = !intf->wpi_reference->Transaction(op.send_data, op.send_size, (uint8_t *)&resp_buf[off], op.recv_size);
			} else if (op.type == 2) {
				// Write
				header.success = !intf->wpi_reference->WriteBulk(op.send_data, op.send_size);
			} else if (op.type == 3) {
				// Read
				header.success = !intf->wpi_reference->Read(op.reg_address, op.recv_size, (uint8_t *)&resp_buf[off]);
			} else if (op.type == 4) {
				// Read Only
				header.success = !intf->wpi_reference->ReadOnly(op.recv_size, (uint8_t *)&resp_buf[off]);
			}

			Toast::IPC::sendto(IO::I2C_IPC::I2C_TRANSACTION_COMPLETE, &resp_buf[0], total_size, op.module_id);
			intf->operations.pop();
		}
		intf->cv.unlock();
	}
	while (!intf->operations.empty()) {
		I2C_OP op = intf->operations.front();
		free(op.send_data);
		intf->operations.pop();
	}
	delete intf->wpi_reference;
	delete intf;
}

static void _msg_handler_init(std::string handle, void *data, int data_len, int module_id, void *param) {
	IO::I2C_IPC::I2CInitMessage *msg = (IO::I2C_IPC::I2CInitMessage *)data;
	if (handle == IO::I2C_IPC::I2C_INIT) {
		for (auto i : interfaces) {
			if (i->port == msg->port && i->addr == msg->addr) {
				i->refcount += 1;
				return;
			}
		}
		I2C_INTERFACE *intf = new I2C_INTERFACE();
		intf->port = msg->port;
		intf->addr = msg->addr;
		intf->refcount = 1;
		intf->wpi_reference = new I2C(intf->port == IO::I2C::Port::MXP ? I2C::Port::kMXP : I2C::Port::kOnboard, intf->addr);
		interfaces.push_back(intf);

		std::thread t(_i2c_threadfunc, intf);
		t.detach();
	} else if (handle == IO::I2C_IPC::I2C_DEINIT) {
		for (auto iter = interfaces.begin(); iter != interfaces.end();) {
			auto i = *iter;

			if (i->port == msg->port && i->addr == msg->addr) {
				i->refcount -= 1;
				if (i->refcount == 0) {
					iter = interfaces.erase(iter);
					i->marked_for_deletion = true;
					i->cv.signal_all();
				}
				return;
			}
			++iter;
		}
	}
}

static void _msg_handler_transaction(std::string handle, void *data, int data_len, int module_id, void *param) {
	IO::I2C_IPC::I2CMessageHeader *header = (IO::I2C_IPC::I2CMessageHeader *)data;

	uint8_t *send_data_buff = header->send_size > 0 ? (uint8_t*)malloc(header->send_size) : NULL;
	if (header->send_size > 0)
		memcpy(send_data_buff, (uint8_t *)data + sizeof(IO::I2C_IPC::I2CMessageHeader), header->send_size);

	for (auto i : interfaces) {
		if (i->port == header->port && i->addr == header->addr) {
			i->operations.push({ header->message_idx, 1, send_data_buff, header->send_size, header->receive_size, 0, module_id });
			i->cv.signal_all();
			return;
		}
	}
}

static void _msg_handler_write(std::string handle, void *data, int data_len, int module_id, void *param) {
	IO::I2C_IPC::I2CMessageHeader *header = (IO::I2C_IPC::I2CMessageHeader *)data;

	uint8_t *send_data_buff = header->send_size > 0 ? (uint8_t*)malloc(header->send_size) : NULL;
	if (header->send_size > 0)
		memcpy(send_data_buff, (uint8_t *)data + sizeof(IO::I2C_IPC::I2CMessageHeader), header->send_size);

	for (auto i : interfaces) {
		if (i->port == header->port && i->addr == header->addr) {
			i->operations.push({ header->message_idx, 2, send_data_buff, header->send_size, 0, 0, module_id });
			i->cv.signal_all();
			return;
		}
	}
}

static void _msg_handler_read(std::string handle, void *data, int data_len, int module_id, void *param) {
	IO::I2C_IPC::I2CMessageHeader *header = (IO::I2C_IPC::I2CMessageHeader *)data;

	for (auto i : interfaces) {
		if (i->port == header->port && i->addr == header->addr) {
			i->operations.push({ header->message_idx, 3, NULL, 0, header->receive_size, header->register_address, module_id });
			i->cv.signal_all();
			return;
		}
	}
}

static void _msg_handler_read_only(std::string, void *data, int data_len, int module_id, void *param) {
	IO::I2C_IPC::I2CMessageHeader *header = (IO::I2C_IPC::I2CMessageHeader *)data;

	for (auto i : interfaces) {
		if (i->port == header->port && i->addr == header->addr) {
			i->operations.push({ header->message_idx, 4, NULL, 0, header->receive_size, 0, module_id });
			i->cv.signal_all();
			return;
		}
	}
}

void init_itf_i2c() {
	Toast::IPC::listen(IO::I2C_IPC::I2C_INIT, _msg_handler_init);
	Toast::IPC::listen(IO::I2C_IPC::I2C_DEINIT, _msg_handler_init);

	Toast::IPC::listen(IO::I2C_IPC::I2C_TRANSACTION, _msg_handler_transaction);
	Toast::IPC::listen(IO::I2C_IPC::I2C_WRITE, _msg_handler_write);
	Toast::IPC::listen(IO::I2C_IPC::I2C_READ, _msg_handler_read);
	Toast::IPC::listen(IO::I2C_IPC::I2C_READ_ONLY, _msg_handler_read_only);
}