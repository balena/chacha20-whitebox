#ifndef CHACHA20_H_
#define CHACHA20_H_

#include <stdint.h>

void chacha20_core_qr(uint32_t x[16]);
void chacha20_core_sum(uint32_t x[16], const uint32_t input[16]);
void chacha20_serialize(uint8_t output[64], uint32_t x[16]);

void chacha20_core(uint8_t output[64], const uint32_t input[16]);

void chacha20_key_expand(uint32_t input[16], const uint8_t key[32], const
    uint8_t nonce[12], uint32_t counter);

void chacha20_block(uint32_t input[16], uint8_t out[64], const uint8_t in[64]);

#endif  /* CHACHA20_H_ */
