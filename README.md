# ChaCha20 whitebox cryptography

This code contains an implementation of the ChaCha20 stream cipher, designed by Daniel J Bernstein and a part of the eSTREAM stream cipher portfolio, RFC 7539 variant (nonce = 96 bits, counter = 32 bits). It is a simple non-optimized version of the specification with focus on testability only.

It requires a minimal set of C and no special external libraries.

The whitebox output supports encrypting of a single 64-bytes block.

## How it works

* `chacha20.c` and `chacha20.h` implement the standard ChaCha20 cipher for 1-block (64 bytes).

* `gen_tables.c` generates a `chacha20_tables.c` which contains the precomputed whitebox tables.

* `chacha20_whitebox.c` consumes the generated `chacha20_tables.c` to produce the cipher function `chacha20_whitebox_block(n, out, in)`, where `n` is the nonce, `out` the 64-bytes block ouput and `in` the 64-bytes block input.

The `gen_tables.c` precalculates tables based on the result of the ChaCha20 computation varying the nonce up to a given limit (13-bits is provided as example) and maintaining same key and counter. The output function `chacha20_core_table` contained in the output file `chacha20_tables.c` does a single lookup based on the input nonce, returning the final mask (16 32-bit words) to be applied to the input.

## Building

A simple `make` performs a complete testing build:

```
$ make
```

It will generate a static library `libchacha20.a`, containing the just the single block ChaCha20 cipher, build its unit test `chacha20_tests` and run it, then build `gen_tests` and execute it, generating a `chacha20_tables.c`. Finally it will generate another static library `libchacha20_whitebox.a` which exports the function `chacha20_whitebox_block` and depends on the `libchacha20.a` (it is a simple dependency on the `chacha20_serialize` function, it is easy to isolate it if needed). And then it will build the `chacha20_whitebox_tests` and run it.

The final unit test compares the output of the generated function with the original code.

In order to decrypt, run the function a second time, using the ciphertext as the input, maintaining the same nonce used to encrypt.

## Security

The security offered by this form of WBC is under evaluation, thus, use it with caution.
