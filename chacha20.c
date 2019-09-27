#include "chacha20.h"

#include <stddef.h>
#include <string.h>

#define U8TO32_LITTLE(p)                              \
  (((uint32_t)((p)[0])) | ((uint32_t)((p)[1]) << 8) | \
   ((uint32_t)((p)[2]) << 16) | ((uint32_t)((p)[3]) << 24))

/* sigma contains the ChaCha constants, which happen to be an ASCII string. */
static const uint8_t sigma[16] = { 'e', 'x', 'p', 'a', 'n', 'd', ' ', '3',
                                   '2', '-', 'b', 'y', 't', 'e', ' ', 'k' };

#define ROTATE(v, n) (((v) << (n)) | ((v) >> (32 - (n))))

#define U32TO8_LITTLE(p, v)    \
  {                            \
    (p)[0] = (v >> 0) & 0xff;  \
    (p)[1] = (v >> 8) & 0xff;  \
    (p)[2] = (v >> 16) & 0xff; \
    (p)[3] = (v >> 24) & 0xff; \
  }

/* QUARTERROUND updates a, b, c, d with a ChaCha "quarter" round. */
#define QUARTERROUND(a, b, c, d) \
  a += b; d = ROTATE(d ^ a, 16); \
  c += d; b = ROTATE(b ^ c, 12); \
  a += b; d = ROTATE(d ^ a,  8); \
  c += d; b = ROTATE(b ^ c,  7);

void chacha20_core_qr(uint32_t x[16]) {
  for (int i = 20; i > 0; i -= 2) {
    QUARTERROUND(x[0], x[4], x[ 8], x[12])
    QUARTERROUND(x[1], x[5], x[ 9], x[13])
    QUARTERROUND(x[2], x[6], x[10], x[14])
    QUARTERROUND(x[3], x[7], x[11], x[15])

    QUARTERROUND(x[0], x[5], x[10], x[15])
    QUARTERROUND(x[1], x[6], x[11], x[12])
    QUARTERROUND(x[2], x[7], x[ 8], x[13])
    QUARTERROUND(x[3], x[4], x[ 9], x[14])
  }
}

void chacha20_core_sum(uint32_t x[16], const uint32_t input[16]) {
  for (int i = 0; i < 16; ++i) {
    x[i] += input[i];
  }
}

void chacha20_serialize(uint8_t output[64], uint32_t x[16]) {
  for (int i = 0; i < 16; ++i) {
    U32TO8_LITTLE(output + 4 * i, x[i]);
  }
}

/* chacha_core performs 20 rounds of ChaCha on the input words in
 * |input| and writes the 64 output bytes to |output|. */
void chacha20_core(uint8_t output[64], const uint32_t input[16]) {
  uint32_t x[16];

  memcpy(x, input, sizeof(uint32_t) * 16);
  chacha20_core_qr(x);
  chacha20_core_sum(x, input);
  chacha20_serialize(output, x);
}

void chacha20_key_expand(uint32_t input[16], const uint8_t key[32], const
    uint8_t nonce[12], uint32_t counter) {
  input[0] = U8TO32_LITTLE(sigma + 0);
  input[1] = U8TO32_LITTLE(sigma + 4);
  input[2] = U8TO32_LITTLE(sigma + 8);
  input[3] = U8TO32_LITTLE(sigma + 12);

  input[4] = U8TO32_LITTLE(key + 0);
  input[5] = U8TO32_LITTLE(key + 4);
  input[6] = U8TO32_LITTLE(key + 8);
  input[7] = U8TO32_LITTLE(key + 12);

  input[8] = U8TO32_LITTLE(key + 16);
  input[9] = U8TO32_LITTLE(key + 20);
  input[10] = U8TO32_LITTLE(key + 24);
  input[11] = U8TO32_LITTLE(key + 28);

  input[12] = counter;
  input[13] = U8TO32_LITTLE(nonce + 0);
  input[14] = U8TO32_LITTLE(nonce + 4);
  input[15] = U8TO32_LITTLE(nonce + 8);
}

void chacha20_block(uint32_t input[16], uint8_t out[64], const uint8_t in[64]) {
  uint8_t buf[64];

  chacha20_core(buf, input);
  for (int i = 0; i < 64; i++) {
    out[i] = in[i] ^ buf[i];
  }

  input[12]++;
}
