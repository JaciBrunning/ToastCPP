#include "thp/wpi/interfaces.hpp"

#include "toast/ipc.hpp"
#include "toast/concurrent/mutex.hpp"
#include "toast/concurrent/condition.hpp"

#include "SPI.h"
#include "io/SPI.hpp"

#include <queue>
#include <thread>

#include <iostream>

// We use this struct in the SPI_INTERFACE operation queue.
// The reason we use a queue is such that multiple read/write
// operations can be done in series, such as .write() or .read() being
// called from multiple threads, or in an async manner.
typedef struct {
	int message_idx, type;
	uint8_t *send_data;
	int size;
	bool initiate;
	int module_id;
} SPI_OP;

typedef struct {
	IO::SPI::Port port;
	int refcount;
	Toast::Concurrent::ConditionVariable cv;
	SPI *wpi_reference;
	bool marked_for_deletion = false;
	std::queue<SPI_OP> operations;
} SPI_INTERFACE;

static SPI_INTERFACE *interfaces[5];	// One for each IO::SPI::Port

static void _spi_threadfunc(SPI_INTERFACE *intf) {
	char resp_buf[512];
	// This will break out of the loop if we've reached a reference count of 0
	while (!intf->marked_for_deletion) {
		intf->cv.lock();
		if (intf->operations.empty()) intf->cv.wait();
		if (!intf->marked_for_deletion && !intf->operations.empty()) {
			SPI_OP op = intf->operations.front();
			IO::SPI_IPC::SPIResponseHeader header = { intf->port, op.size, /* ret_val */ 0, op.message_idx };
			int off = sizeof(header);
			int total_size = off + header.size;

			if (op.type == 1) {
				// Read
				header.ret_val = intf->wpi_reference->Read(op.initiate, (uint8_t *)&resp_buf[off], op.size);
			} else if (op.type == 2) {
				// Write
				header.ret_val = intf->wpi_reference->Write(op.send_data, op.size);
			} else if (op.type == 3) {
				// Transaction
				header.ret_val = intf->wpi_reference->Transaction(op.send_data, (uint8_t *)&resp_buf[off], op.size);
			}

			memcpy(&resp_buf[0], &header, sizeof(header));
			Toast::IPC::sendto(IO::SPI_IPC::SPI_TRANSACTION_COMPLETE, &resp_buf[0], total_size, op.module_id);
			intf->operations.pop();
		}
		intf->cv.unlock();
	}
	while (!intf->operations.empty()) {
		SPI_OP op = intf->operations.front();
		if (op.send_data != NULL)
			free(op.send_data);
		intf->operations.pop();
	}

	delete intf->wpi_reference;
	delete intf;
}

static void _msg_handler_init(std::string handle, void *data, int data_len, int module_id, void *param) {
	IO::SPI::Port port = *(IO::SPI::Port *)data;
	if (handle == IO::SPI_IPC::SPI_INIT) {
		if (interfaces[(int)port] != NULL) {
			interfaces[(int)port]->refcount += 1;
		} else {
			SPI_INTERFACE *intf = new SPI_INTERFACE();
			intf->port = port;
			intf->refcount = 1;
			intf->wpi_reference = new SPI((SPI::Port)(int)port);
			intf->marked_for_deletion = false;
			interfaces[(int)port] = intf;

			std::thread t(_spi_threadfunc, intf);
			t.detach();
		}
	} else if (handle == IO::SPI_IPC::SPI_DEINIT) {
		if (interfaces[(int)port] != NULL) {
			SPI_INTERFACE *intf = interfaces[(int)port];
			intf->refcount -= 1;
			if (intf->refcount == 0) {
				interfaces[(int)port] = NULL;
				intf->marked_for_deletion = true;
				intf->cv.signal_all();
			}
		}
	}
}

static void _msg_handler_prop(std::string handle, void *data, int data_len, int module_id, void *param) {
	IO::SPI_IPC::SPIPropertySet *msg = (IO::SPI_IPC::SPIPropertySet *)data;

	float val = msg->val;

	if (interfaces[(int)msg->port] != NULL) {
		SPI_INTERFACE *intf = interfaces[(int)msg->port];
		SPI *wpi = intf->wpi_reference;

		switch (msg->prop) {
		case 1:	// Clock Rate
			wpi->SetClockRate(val);
			break;
		case 2:	// LSB/MSB First
			if (val == 1)	wpi->SetMSBFirst();
			else			wpi->SetLSBFirst();
			break;
		case 3: // Sample Data Edge
			if (val == 1)	wpi->SetSampleDataOnFalling();
			else			wpi->SetSampleDataOnRising();
			break;
		case 4:	// Clock Active Edge
			if (val == 1)	wpi->SetClockActiveLow();
			else			wpi->SetClockActiveHigh();
			break;
		case 5: // Chip Select Active Edge
			if (val == 1)	wpi->SetChipSelectActiveLow();
			else			wpi->SetChipSelectActiveHigh();
			break;
		}
	}
}

static void _msg_handler(std::string handle, void *data, int data_len, int module_id, void *param) {
	IO::SPI_IPC::SPIMessageHeader *header = (IO::SPI_IPC::SPIMessageHeader *)data;
	// Storing an integer as a pointer address, cheeky but it works.
	uint8_t type = (uint8_t)(uintptr_t)(param);

	uint8_t *send_data_buff = NULL;
	// Write or Transaction
	if ((type == 2 || type == 3) && header->size > 0) {
		send_data_buff = (uint8_t *)malloc(header->size);
		memcpy(send_data_buff, (uint8_t *)data + sizeof(IO::SPI_IPC::SPIMessageHeader), header->size);
	}

	if (interfaces[(int)header->port] != NULL) {
		SPI_INTERFACE *intf = interfaces[(int)header->port];
		intf->operations.push({ header->message_idx, type, send_data_buff, header->size, header->initiate, module_id });
		intf->cv.signal_all();
	}
}

void init_itf_spi() {
	Toast::IPC::listen(IO::SPI_IPC::SPI_INIT, _msg_handler_init);
	Toast::IPC::listen(IO::SPI_IPC::SPI_DEINIT, _msg_handler_init);

	Toast::IPC::listen(IO::SPI_IPC::SPI_PROPERTY_SET, _msg_handler_prop);

	Toast::IPC::listen(IO::SPI_IPC::SPI_READ, _msg_handler, (void *)(uintptr_t)1);
	Toast::IPC::listen(IO::SPI_IPC::SPI_WRITE, _msg_handler, (void *)(uintptr_t)2);
	Toast::IPC::listen(IO::SPI_IPC::SPI_TRANSACTION, _msg_handler, (void *)(uintptr_t)3);
}