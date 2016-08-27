#include "toast/math/buffer.hpp"

#include <stdlib.h>
#include <string.h>

using namespace Toast::Math::Buffer;

FastHistoryBuffer::FastHistoryBuffer(int size, int element_size) {
	_rot_i = 0;
	_size = size;
	_el_size = element_size;
	_full_cycle = false;

	_buf = (char *)malloc(_size * _el_size);
	memset(_buf, 0, _size * _el_size);
}

FastHistoryBuffer::FastHistoryBuffer(FastHistoryBuffer &other) {
	_rot_i = other._rot_i;
	_size = other._size;
	_el_size = other._el_size;
	_full_cycle = other._full_cycle;

	_buf = (char *)malloc(_size * _el_size);
	memcpy(_buf, other._buf, _size * _el_size);
}

FastHistoryBuffer::~FastHistoryBuffer() {
	free(_buf);
}

int FastHistoryBuffer::size() {
	return _size;
}

int FastHistoryBuffer::element_size() {
	return _el_size;
}

int FastHistoryBuffer::total_size() {
	return _size * _el_size;
}

void FastHistoryBuffer::push(char *element) {
	memcpy(_buf + (_rot_i * _el_size), element, _el_size);
	_rot_i++;
	if (_rot_i >= _size) {
		_full_cycle = true;
		_rot_i = 0;
	}
}

void FastHistoryBuffer::get(int relative_index, char *buffer) {
	int start_idx = relative_index;
	if (_full_cycle) start_idx = _rot_i + relative_index;
	start_idx %= _size;
	memcpy(buffer, _buf + (start_idx * _el_size), _el_size);
}