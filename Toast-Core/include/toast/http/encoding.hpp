#include "toast/library.hpp"
#include <string>

namespace Toast {
	namespace HTTP {
        namespace Encoding {
            API std::string base64_encode(const char *data, int length);
        }
    }
}