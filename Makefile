CC=m68k-amigaos-gcc
CFLAGS = -DWOLFSSL_USER_SETTINGS
CFLAGS += -O2 -mcrt=nix20 -Wno-error=cpp -Wno-missing-braces -mcpu=68030 -m68020-60 -lm -fomit-frame-pointer -flto
CFLAGS += -Iwolfssl -I.

OBJECTS := wolfssl/wolfcrypt/src/rsa.o \
	wolfssl/wolfcrypt/src/asn.o \
	wolfssl/wolfcrypt/src/aes.o \
	wolfssl/wolfcrypt/src/ecc.o \
	wolfssl/wolfcrypt/src/chacha.o \
	wolfssl/wolfcrypt/src/poly1305.o \
	wolfssl/wolfcrypt/src/chacha20_poly1305.o \
	wolfssl/wolfcrypt/src/dh.o \
	wolfssl/wolfcrypt/src/sha.o \
	wolfssl/wolfcrypt/src/sha256.o \
	wolfssl/wolfcrypt/src/sha512.o \
	wolfssl/wolfcrypt/src/integer.o \
	wolfssl/wolfcrypt/src/tfm.o \
	wolfssl/wolfcrypt/src/random.o \
	wolfssl/wolfcrypt/src/logging.o \
	wolfssl/wolfcrypt/src/memory.o \
	wolfssl/wolfcrypt/src/coding.o \
	wolfssl/wolfcrypt/src/hash.o \
	wolfssl/wolfcrypt/src/kdf.o \
	wolfssl/wolfcrypt/src/hmac.o \
	wolfssl/wolfcrypt/src/md5.o \
	wolfssl/wolfcrypt/src/wc_port.o \
	wolfssl/wolfcrypt/src/wc_encrypt.o \
	wolfssl/wolfcrypt/src/wolfmath.o \
	wolfssl/wolfcrypt/src/sp_c32.o \
	wolfssl/wolfcrypt/src/sp_int.o \
	wolfssl/wolfcrypt/src/signature.o \
	wolfssl/wolfcrypt/src/error.o \
	wolfssl/wolfcrypt/benchmark/benchmark.o \
	wolfssl/wolfcrypt/test/test.o \
	amirng.o

BENCH_OBJECTS := bench.o

TEST_OBJECTS := test.o

all: wolfbench wolftest

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

wolfbench: $(OBJECTS) $(BENCH_OBJECTS)
	$(CC) $(OBJECTS) $(BENCH_OBJECTS) $(CFLAGS) -o $@

wolftest: $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(CFLAGS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f wolfbench
	-rm -f wolftest

check:
	vamos -C 68040 -m4096 -s32 wolftest
