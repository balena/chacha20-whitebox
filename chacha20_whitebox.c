#include "chacha20.h"

#include <string.h>
#include <assert.h>

#include "chacha20_tables.c"

void chacha20_whitebox_block(uint16_t nonce, uint8_t out[64], const uint8_t in[64]) {
  uint8_t buf[64];
  uint32_t x[16];

  chacha20_core_table(x, nonce);
  chacha20_serialize(buf, x);

  for (int i = 0; i < 64; i++) {
    out[i] = in[i] ^ buf[i];
  }
}
