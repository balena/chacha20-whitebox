CFLAGS = -O3

.PHONY: clean all

all: chacha20_tests gen_tables chacha20_whitebox_tests

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

libchacha20.a: chacha20.o
	$(AR) $(ARFLAGS) $@ $^

chacha20_tests: chacha20_tests.c libchacha20.a
	$(CC) $(CFLAGS) $^ -o $@
	./$@

gen_tables: gen_tables.o libchacha20.a
	$(CC) $(CFLAGS) $^ -o $@
	./$@

chacha20_whitebox_tests: chacha20.o chacha20_whitebox.o \
		chacha20_whitebox_tests.c
	$(CC) $(CFLAGS) $^ -o $@
	./$@

clean:
	rm -f *.o *.a chacha20_tests gen_tables chacha20_whitebox_tests chacha20_tables.c
