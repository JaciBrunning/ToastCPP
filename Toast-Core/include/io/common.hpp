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

#define SET_BIT(expression, bit) (expression |= (1 << bit))
#define UNSET_BIT(expression, bit) (expression &= ~(1 << bit))
#define SET_BIT_TO(expression, bit, val) (expression ^= (-(val ? 1 : 0) ^ expression) & (1 << bit))
#define IS_BIT_SET(expression, bit) ((expression & (1 << bit)) != 0)

#define MEM_VAL(type, ptr, offset) *(type *)(ptr + offset)

#define SET_BIT_MULTIWIDE(type, ptr, offset, bit) SET_BIT(*(type *)(ptr + offset), bit)
#define UNSET_BIT_MULTIWIDE(type, ptr, offset, bit) UNSET_BIT(*(type *)(ptr + offset), bit)
#define SET_BIT_TO_MULTIWIDE(type, ptr, offset, bit, val) SET_BIT_TO(*(type *)(ptr + offset), bit, val)
#define IS_BIT_SET_MULTIWIDE(type, ptr, offset, bit) IS_BIT_SET(*(type *)(ptr + offset), bit)

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

	class CouldNotAllocateBlockException : public std::exception {
	public:
		CouldNotAllocateBlockException(std::string message) : std::exception(message.c_str()) {}
	};
}