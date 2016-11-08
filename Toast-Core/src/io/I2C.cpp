#include "io/I2C.hpp"
#include "toast/ipc.hpp"
#include "toast/concurrent/condition.hpp"

#include <map>

using namespace IO;

I2C::I2C(I2C::Port _port, int device_address) : port(_port), addr(device_address) {
	I2C_IPC::I2CInitMessage msg = { port, addr };
	Toast::IPC::send(I2C_IPC::I2C_INIT, &msg, sizeof(msg));
}

I2C::~I2C() {
	I2C_IPC::I2CInitMessage msg = { port, addr };
	Toast::IPC::send(I2C_IPC::I2C_DEINIT, &msg, sizeof(msg));
}

static uint8_t data_buffer[I2C_MAX_TRANSACTION + sizeof(I2C_IPC::I2CMessageHeader)];
static Toast::Concurrent::Mutex buffer_mtx;
static int message_idx = 0;
static std::map<int, I2C::TransactionCallback> callbacks;

static bool _listen_transaction_registered = false;
static void _listen_transaction(std::string handle, void *data, int data_length, int module_id, void *param) {
	I2C_IPC::I2CResponseHeader *rheader = (I2C_IPC::I2CResponseHeader *)(data);
	uint8_t *read_data = (rheader->length == 0 ? NULL : (uint8_t *)data + data_length - sizeof(I2C_IPC::I2CResponseHeader));
	I2C::TransactionData tdata = { rheader->length, rheader->success, rheader->port, rheader->addr, read_data };
	if (callbacks[rheader->message_idx] != NULL)
		callbacks[rheader->message_idx](tdata);
	callbacks.erase(rheader->message_idx);
}

static void listen_reg(std::string handle) {
	if (!_listen_transaction_registered) {
		Toast::IPC::listen(handle, _listen_transaction);
		_listen_transaction_registered = true;
	}
}

static void init_message_header(I2C::Port port, int addr, int send_size, int recv_size, int message_idx, int register_address = 0) {
	I2C_IPC::I2CMessageHeader *header = (I2C_IPC::I2CMessageHeader *)&data_buffer[0];
	header->port = port;
	header->addr = addr;
	header->send_size = send_size;
	header->receive_size = recv_size;
	header->message_idx = message_idx;
	header->register_address = register_address;
}

void I2C::transaction(uint8_t *send_data, int send_size, int receive_size, TransactionCallback callback) {
	listen_reg(I2C_IPC::I2C_TRANSACTION_COMPLETE);
	int datalen = sizeof(I2C_IPC::I2CMessageHeader) + send_size;
	int mid = message_idx++;
	callbacks[mid] = callback;
	buffer_mtx.lock();
	init_message_header(port, addr, send_size, receive_size, mid);
	if (send_size > 0)
		memcpy(&data_buffer[0] + sizeof(I2C_IPC::I2CMessageHeader), send_data, send_size);
	Toast::IPC::send(I2C_IPC::I2C_TRANSACTION, &data_buffer[0], datalen);
	buffer_mtx.unlock();
}

bool I2C::transaction(uint8_t *send_data, int send_size, uint8_t *recv_data, int recv_size) {
	Toast::Concurrent::ConditionVariable cv;
	bool success = false;
	I2C::transaction(send_data, send_size, recv_size, [&cv, &success, recv_data](TransactionData d) {
		success = d.success;
		memcpy(recv_data, d.data, d.length);
		cv.signal();
	});
	cv.lock();
	cv.wait();
	cv.unlock();
	return !success;
}

void I2C::write(int register_address, uint8_t data, TransactionCallback cb) {
	uint8_t buffer[2] = { (uint8_t)register_address, data };
	I2C::write_bulk(&buffer[0], 2, cb);
}

bool I2C::write(int register_address, uint8_t data) {
	uint8_t buffer[2] = { (uint8_t) register_address, data };
	return I2C::write_bulk(&buffer[0], 2);
}

void I2C::write_bulk(uint8_t *data, int count, TransactionCallback cb) {
	listen_reg(I2C_IPC::I2C_WRITE_COMPLETE);
	int data_len = sizeof(I2C_IPC::I2CMessageHeader) + count;
	int mid = message_idx++;
	callbacks[mid] = cb;
	buffer_mtx.lock();
	init_message_header(port, addr, count, 0, mid);
	memcpy(&data_buffer[0] + sizeof(I2C_IPC::I2CMessageHeader), data, count);
	Toast::IPC::send(I2C_IPC::I2C_WRITE, &data_buffer[0], data_len);
	buffer_mtx.unlock();
}

bool I2C::write_bulk(uint8_t *data, int count) {
	Toast::Concurrent::ConditionVariable cv;
	bool success = false;
	I2C::write_bulk(data, count, [&cv, &success](TransactionData d) {
		success = d.success;
		cv.signal();
	});
	cv.lock();
	cv.wait();
	cv.unlock();
	return !success;
}

void I2C::read(int register_address, int count, TransactionCallback callback) {
	listen_reg(I2C_IPC::I2C_READ_COMPLETE);
	int mid = message_idx++;
	callbacks[mid] = callback;
	buffer_mtx.lock();
	init_message_header(port, addr, 0, count, mid, register_address);
	Toast::IPC::send(I2C_IPC::I2C_READ, &data_buffer[0], sizeof(I2C_IPC::I2CMessageHeader));
	buffer_mtx.unlock();
}

bool I2C::read(int register_address, int count, uint8_t *data) {
	Toast::Concurrent::ConditionVariable cv;
	bool success = false;
	I2C::read(register_address, count, [&cv, &success, data](TransactionData d) {
		success = d.success;
		memcpy(data, d.data, d.length);
		cv.signal();
	});
	cv.lock();
	cv.wait();
	cv.unlock();
	return !success;
}

void I2C::read_only(int count, TransactionCallback callback) {
	listen_reg(I2C_IPC::I2C_READ_COMPLETE);
	int mid = message_idx++;
	callbacks[mid] = callback;
	buffer_mtx.lock();
	init_message_header(port, addr, 0, count, mid);
	Toast::IPC::send(I2C_IPC::I2C_READ_ONLY, &data_buffer[0], sizeof(I2C_IPC::I2CMessageHeader));
	buffer_mtx.unlock();
}

bool I2C::read_only(int count, uint8_t *data) {
	Toast::Concurrent::ConditionVariable cv;
	bool success = false;
	I2C::read_only(count, [&cv, &success, data](TransactionData d) {
		success = d.success;
		memcpy(data, d.data, d.length);
		cv.signal();
	});
	cv.lock();
	cv.wait();
	cv.unlock();
	return !success;
}