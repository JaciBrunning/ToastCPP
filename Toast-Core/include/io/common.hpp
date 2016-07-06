#pragma once

#include <exception>
#include <string>

#define PORT_CHECK(port, max_num)		\
	if (port >= max_num || port < 0) {	\
		throw IO::PortOutOfBoundException(max_num, port); \
	}

#define BLOCK_CHECK(id, max_id)			\
	if (id >= max_id || id < 0) {	\
		throw IO::PortOutOfBoundException(max_id, id); \
	}

namespace IO {
	class PortOutOfBoundException : public std::exception {
	public:
		PortOutOfBoundException(std::string message) : std::exception(message.c_str()) {}
		PortOutOfBoundException(int expected, int given)
			: PortOutOfBoundException("Port out of bounds! Expected less than " + std::to_string(expected) + 
				", got " + std::to_string(given)) { }
	};

	class BlockOutOfBoundException : public std::exception {
	public:
		BlockOutOfBoundException(std::string message) : std::exception(message.c_str()) {}
		BlockOutOfBoundException(int expected, int given)
			: BlockOutOfBoundException("Block out of bounds! Expected less than " + std::to_string(expected) + 
				", got " + std::to_string(given) + " (block not allocated for id)") { }
	};
}