#pragma once

#include "toast/library.hpp"

namespace Toast {
	namespace Math {
		namespace Lookup {
			struct BinaryLookup {
				API BinaryLookup(int size);
				API virtual ~BinaryLookup();

				API int size();
				API float get_first(int id);
				API float get_second(int id);

				API void insert(int id, float first, float second);
				API void find_between(float value, int *low, int *high, bool inverse = false);

				API float interpolate(float first);
				API float interpolate_inverse(float second);

			private:
				float *_store;
				int _size;
			};
		}
	}
}