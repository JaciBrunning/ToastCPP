#include "toast/net/transport.hpp"
#include <string.h>

using namespace Toast::Net;

void Transport::intToBytes(int n, char *bytes, int a) {
    bytes[a+0] = (n >> 24) & 0xFF;
    bytes[a+1] = (n >> 16) & 0xFF;
    bytes[a+2] = (n >> 8) & 0xFF;
    bytes[a+3] = n & 0xFF;
}

int Transport::bytesToInt(char *bytes, int a) {
    int value = 0;
    int i;
    for (i = 0; i < 4; i++) {
        int shift = (4 - 1 - i) * 8;
        value += (bytes[a+i] & 0x000000FF) << shift;
    }
    return value;
}

void Transport::floatToBytes(float f, char *buf, int start) {
    Transport::intToBytes(floatToInt(f), buf, start);
}

float Transport::bytesToFloat(char *buf, int start) {
    return Transport::intToFloat(bytesToInt(buf, start));
}

float Transport::intToFloat(int i) {
    float result;
    memcpy(&result, &i, 4);
    return result;
}

int Transport::floatToInt(float i) {
    int result;
    memcpy(&result, &i, 4);
    return result;
}