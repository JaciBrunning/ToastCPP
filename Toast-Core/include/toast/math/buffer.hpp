#pragma once

#include "toast/library.hpp"

namespace Toast {
	namespace Math {
		namespace Buffer {
			struct FastHistoryBuffer {
				API FastHistoryBuffer(int size, int element_size);
				API FastHistoryBuffer(FastHistoryBuffer &to_copy);
				API virtual ~FastHistoryBuffer();

				API int size();
				API int element_size();
				API int total_size();
				
				API void push(char *element);
				API void get(int relative_index, char *buffer);
			
			private:
				int _size, _el_size, _rot_i;
				bool _full_cycle = false;
				char *_buf;
			};
		}
	}
}