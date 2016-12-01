#include "io/SPI.hpp"
#include "toast/ipc.hpp"
#include "toast/concurrent/condition.hpp"

#include <map>
#include <iostream>

using namespace IO;

SPI::SPI(SPI::Port _port) : port(_port) {
	Toast::IPC::send(SPI_IPC::SPI_INIT, &port, sizeof(port));
}

SPI::~SPI() {
	Toast::IPC::send(SPI_IPC::SPI_DEINIT, &port, sizeof(port));
}

static uint8_t data_buffer[SPI_MAX_TRANSACTION + sizeof(SPI_IPC::SPIMessageHeader)];
static Toast::Concurrent::Mutex buffer_mtx;
static int message_idx = 0;
static std::map<int, SPI::SPICallback> callbacks;

static bool _listen_reg = false;

static void _listen_func(std::string handle, void *data, int data_len, int module_id, void *param) {
	buffer_mtx.lock();
	SPI_IPC::SPIResponseHeader *rheader = (SPI_IPC::SPIResponseHeader *)(data);
	uint8_t *read_data = (rheader->size == 0 ? NULL : (uint8_t *)data + sizeof(SPI_IPC::SPIResponseHeader));
	SPI::SPIData sdata = { rheader->port, rheader->ret_val, read_data };
	if (callbacks[rheader->message_idx] != NULL) {
		callbacks[rheader->message_idx](sdata);
		callbacks.erase(rheader->message_idx);
	}
	buffer_mtx.unlock();
}

static void listen_reg() {
	if (!_listen_reg) {
		Toast::IPC::listen(SPI_IPC::SPI_TRANSACTION_COMPLETE, _listen_func);
		_listen_reg = true;
	}
}

static void send_property(SPI::Port port, uint8_t prop, float value) {
	SPI_IPC::SPIPropertySet msg = { port, prop, value };
	Toast::IPC::send_with_ack(SPI_IPC::SPI_PROPERTY_SET, &msg, sizeof(msg));
}

static void init_message_header(SPI::Port port, int size, bool initiate, int message_idx) {
	SPI_IPC::SPIMessageHeader *header = (SPI_IPC::SPIMessageHeader *)&data_buffer[0];
	header->port = port;
	header->size = size;
	header->initiate = initiate;
	header->message_idx = message_idx;
}

void SPI::set_clock_rate(double hz) const {
	send_property(port, 1, hz);
}

void SPI::set_msb_first() const {
	send_property(port, 2, 1);
}

void SPI::set_lsb_first() const {
	send_property(port, 2, 2);
}

void SPI::set_sample_data_on_falling() const {
	send_property(port, 3, 1);
}

void SPI::set_sample_data_on_rising() const {
	send_property(port, 3, 2);
}

void SPI::set_clock_active_low() const {
	send_property(port, 4, 1);
}

void SPI::set_clock_active_high() const {
	send_property(port, 4, 2);
}

void SPI::set_chip_select_active_low() const {
	send_property(port, 5, 1);
}

void SPI::set_chip_select_active_high() const {
	send_property(port, 5, 2);
}

void SPI::write(uint8_t *data, int size, SPICallback callback) const {
	listen_reg();
	buffer_mtx.lock();
	int datalen = sizeof(SPI_IPC::SPIMessageHeader) + size;
	int mid = message_idx++;
	callbacks[mid] = callback;
	init_message_header(port, size, false, mid);
	if (size > 0)
		memcpy(&data_buffer[0] + sizeof(SPI_IPC::SPIMessageHeader), data, size);
	Toast::IPC::send(SPI_IPC::SPI_WRITE, &data_buffer[0], datalen);
	buffer_mtx.unlock();
}

int SPI::write(uint8_t *data, int size) const {
	Toast::Concurrent::ConditionVariable cv(true);
	int ret = 0;
	SPI::write(data, size, [&cv, &ret](SPIData sdata) {
		ret = sdata.ret_val;
		cv.signal();
	});
	cv.lock();
	cv.wait();
	cv.unlock();
	return ret;
}

void SPI::read(bool initiate, int size, SPICallback callback) const {
	listen_reg();
	buffer_mtx.lock();
	int mid = message_idx++;
	callbacks[mid] = callback;
	init_message_header(port, size, initiate, mid);
	Toast::IPC::send(SPI_IPC::SPI_READ, &data_buffer[0], sizeof(SPI_IPC::SPIMessageHeader));
	buffer_mtx.unlock();
}

int SPI::read(bool initiate, uint8_t *data, int size) const {
	Toast::Concurrent::ConditionVariable cv(true);
	int ret = 0;
	SPI::read(initiate, size, [&cv, &ret, &data, size](SPIData sdata) {
		ret = sdata.ret_val;
		memcpy(data, sdata.data, size);
		cv.signal();
	});
	cv.lock();
	cv.wait();
	cv.unlock();
	return ret;
}

void SPI::transaction(uint8_t *send_data, int size, SPICallback callback) const {
	listen_reg();
	buffer_mtx.lock();
	int datalen = sizeof(SPI_IPC::SPIMessageHeader) + size;
	int mid = message_idx++;
	callbacks[mid] = callback;
	init_message_header(port, size, false, mid);
	if (size > 0)
		memcpy(&data_buffer[0] + sizeof(SPI_IPC::SPIMessageHeader), send_data, size);
	Toast::IPC::send(SPI_IPC::SPI_TRANSACTION, &data_buffer[0], datalen);
	buffer_mtx.unlock();
}

int SPI::transaction(uint8_t *send_data, uint8_t *read_data, int size) const {
	Toast::Concurrent::ConditionVariable cv(true);
	int ret = 0;
	SPI::transaction(send_data, size, [&cv, &ret, &read_data, size](SPIData sdata) {
		ret = sdata.ret_val;
		memcpy(read_data, sdata.data, size);
		cv.signal();
	});
	cv.lock();
	cv.wait();
	cv.unlock();
	return ret;
}