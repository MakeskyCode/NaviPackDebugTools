#ifndef _COMPRESS_H_
#define _COMPRESS_H_
#include <inttypes.h>

int RunLenghEncoding(void* dataIn, uint32_t inLen, void* dataOut, uint8_t oneMaxLen);
int RunLenghDecoding(const void* dataIn, uint32_t inLen, void* dataOut, uint32_t maxOutLen);

#endif