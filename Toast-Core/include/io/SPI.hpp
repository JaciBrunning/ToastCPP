#pragma once

#include "toast/library.hpp"
#include <string>
#include <stdint.h>
#include <functional>

// 256 Bytes
#define SPI_MAX_TRANSACTION 256

namespace IO {
	// TODO Interrupt Bitbang, minimum delay
	class SPI {
	public:
		enum class Port {
			OnboardCS0 = 0,
			OnboardCS1,
			OnboardCS2,
			OnboardCS3,
			MXP
		};
		struct SPIData {
			Port port;
			int ret_val;
			uint8_t *data;
		};
		typedef std::function<void(SPIData)> SPICallback;

		API SPI(Port port);
		API virtual ~SPI();

		API void set_clock_rate(double hz) const;
		
		API void set_msb_first() const;
		API void set_lsb_first() const;

		API void set_sample_data_on_falling() const;
		API void set_sample_data_on_rising() const;

		API void set_clock_active_low() const;
		API void set_clock_active_high() const;

		API void set_chip_select_active_low() const;
		API void set_chip_select_active_high() const;

		API void write(uint8_t *data, int size, SPICallback callback) const;
		API int write(uint8_t *data, int size) const;

		API void read(bool initiate, int size, SPICallback callback) const;
		API int read(bool initiate, uint8_t *data, int size) const;

		API void transaction(uint8_t *send_data, int size, SPICallback callback) const;
		API int transaction(uint8_t *send_data, uint8_t *read_data, int size) const;

	private:
		Port port;
	};

	namespace SPI_IPC {
		const std::string SPI_INIT = "io.spi.init";
		const std::string SPI_DEINIT = "io.spi.deinit";
		
		const std::string SPI_PROPERTY_SET = "io.spi.property.set";

		const std::string SPI_WRITE = "io.spi.write";

		const std::string SPI_READ = "io.spi.read";

		const std::string SPI_TRANSACTION = "io.spi.transaction";
		// Transaction Complete accounts for everything (write, read, transaction)
		// due to the implementation of message_idx
		const std::string SPI_TRANSACTION_COMPLETE = "io.spi.transaction.complete";

		struct SPIPropertySet {
			SPI::Port port;
			uint8_t prop;
			float val;
		};

		struct SPIMessageHeader {
			SPI::Port port;
			int size;
			bool initiate;
			int message_idx;
		};

		struct SPIResponseHeader {
			SPI::Port port;
			int size, ret_val, message_idx;
		};
	}
}
