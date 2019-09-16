#include <stdio.h>
#include <string.h>

#include "chacha20.h"

int main() {
  const size_t nbits = 13;
  const uint32_t counter = 0;
  uint8_t nonce[12] = {0};
  FILE *tables;

#if 0
  uint8_t key[32] = {0};
  FILE* r = fopen("/dev/random", "r");
  fread(key, 1, 32, r);
  fclose(r);
#else
  const uint8_t key[32] = {
    0x98, 0xbe, 0xf1, 0x46, 0x9b, 0xe7, 0x26, 0x98, 0x37, 0xa4, 0x5b,
    0xfb, 0xc9, 0x2a, 0x5a, 0x6a, 0xc7, 0x62, 0x50, 0x7c, 0xf9, 0x64,
    0x43, 0xbf, 0x33, 0xb9, 0x6b, 0x1b, 0xd4, 0xc6, 0xf8, 0xf6
  };
#endif

  printf("0x"
      "%02x%02x%02x%02x"
      "%02x%02x%02x%02x"
      "%02x%02x%02x%02x"
      "%02x%02x%02x%02x"
      "\n",
      key[ 0], key[ 1], key[ 2], key[ 3],
      key[ 4], key[ 5], key[ 6], key[ 7],
      key[ 8], key[ 9], key[10], key[11],
      key[12], key[13], key[14], key[15]);

  printf("Generating chacha20_tables.c...\n");

  tables = fopen("chacha20_tables.c", "w");
  fprintf(tables, "/* Automatically generated, do not modify */\n\n");
  fprintf(tables, "static void chacha20_core_table(uint32_t x[16], uint16_t nonce) {\n");
  fprintf(tables, "  assert(nonce < %d);\n\n", 1 << nbits);
  fprintf(tables, "  switch (nonce) {");
  for (size_t i = 0; i < (1 << nbits); i++) {
    uint32_t s[16], x[16];

    chacha20_key_expand(s, key, nonce, counter);

    memcpy(x, s, sizeof(uint32_t) * 16);
    chacha20_core_qr(x);
    chacha20_core_sum(x, s);

    fprintf(tables, "\n    case %zd:", i);
    for (size_t j = 0; j < 16; j++) {
      if (j % 2 == 0) {
        fprintf(tables, "\n      ");
      } else {
        fprintf(tables, " ");
      }
      fprintf(tables, "x[%*zu] = 0x%08x;", 2, j, x[j]);
    }
    fprintf(tables, "\n      break;\n");

    nonce[11] += 1;
    if (nonce[11] == 0)
      nonce[10] += 1;
  }
  fprintf(tables, "  }\n");
  fprintf(tables, "}\n");
  fclose(tables);

  printf("Done!\n");

  return 0;
}
