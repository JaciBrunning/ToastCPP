#pragma once

#include "toast/library.hpp"
#include <string>
#include <stdint.h>
#include <functional>

// 256 Bytes
#define I2C_MAX_TRANSACTION 256

namespace IO {
	class I2C {
	public:
		enum class Port {
			Onboard = 0,
			MXP = 1
		};
		struct TransactionData {
			int length;
			bool success;
			Port port;
			int device_address;
			uint8_t *data;
		};
		typedef std::function<void(TransactionData)> TransactionCallback;

		API I2C(Port port, int device_address);
		API virtual ~I2C();

		API void transaction(uint8_t *send_data, int send_size, int receive_size, TransactionCallback callback);
		API bool transaction(uint8_t *send_data, int send_size, uint8_t *read_data, int read_size);

		API void write(int register_address, uint8_t data, TransactionCallback callback);
		API bool write(int register_address, uint8_t data);
		API void write_bulk(uint8_t *data, int count, TransactionCallback callback);
		API bool write_bulk(uint8_t *data, int count);

		API void read(int register_address, int count, TransactionCallback callback);
		API bool read(int register_address, int count, uint8_t *data);

		API void read_only(int count, TransactionCallback callback);
		API bool read_only(int count, uint8_t *data);

	private:
		Port port;
		int addr;
	};

	namespace I2C_IPC {
		const std::string I2C_INIT = "io.i2c.init";
		const std::string I2C_DEINIT = "io.i2c.deinit";

		const std::string I2C_TRANSACTION = "io.i2c.transaction";
		const std::string I2C_TRANSACTION_COMPLETE = "io.i2c.transaction.complete";

		const std::string I2C_WRITE = "io.i2c.write";
		const std::string I2C_WRITE_COMPLETE = "io.i2c.write.complete";

		const std::string I2C_READ = "io.i2c.read";
		const std::string I2C_READ_ONLY = "io.i2c.read.only";
		const std::string I2C_READ_COMPLETE = "io.i2c.read.complete";

		struct I2CInitMessage {
			I2C::Port port;
			int addr;
		};

		struct I2CMessageHeader {
			I2C::Port port;
			int addr, send_size, receive_size, message_idx;
			int register_address = 0;	// This is only used for a Read transaction
		};

		struct I2CResponseHeader {
			I2C::Port port;
			int addr, length, message_idx;
			bool success;
		};
	}
}
