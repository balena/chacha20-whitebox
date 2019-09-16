#ifndef CHACHA20_WHITEBOX_H_
#define CHACHA20_WHITEBOX_H_

#include <stdint.h>

void chacha20_whitebox_block(uint16_t nonce, uint8_t out[64], const uint8_t in[64]);

#endif /* CHACHA20_WHITEBOX_H_ */

