#pragma once

#include "toast/library.hpp"

namespace Toast {
    namespace Net {
        namespace Transport {
            API void intToBytes(int n, char *bytes, int start);
            API int bytesToInt(char *bytes, int start);
            
            API void floatToBytes(float f, char *buf, int start);
            API float bytesToFloat(char *buf, int start);
            
            API float intToFloat(int i);
            API int floatToInt(float i);
        }
    }
}