CC=m68k-amigaos-gcc
CFLAGS = -DWOLFSSL_USER_SETTINGS
CFLAGS += -O2 -mcrt=nix20 -Wno-error=cpp -Wno-missing-braces -m68030 -lm
CFLAGS += -Iwolfssl -I.

CSRCS := wolfssl/wolfcrypt/src/rsa.c \
	wolfssl/wolfcrypt/src/asn.c \
	wolfssl/wolfcrypt/src/aes.c \
	wolfssl/wolfcrypt/src/ecc.c \
	wolfssl/wolfcrypt/src/chacha.c \
	wolfssl/wolfcrypt/src/poly1305.c \
	wolfssl/wolfcrypt/src/chacha20_poly1305.c \
	wolfssl/wolfcrypt/src/dh.c \
	wolfssl/wolfcrypt/src/sha.c \
	wolfssl/wolfcrypt/src/sha256.c \
	wolfssl/wolfcrypt/src/sha512.c \
	wolfssl/wolfcrypt/src/integer.c \
	wolfssl/wolfcrypt/src/tfm.c \
	wolfssl/wolfcrypt/src/random.c \
	wolfssl/wolfcrypt/src/logging.c \
	wolfssl/wolfcrypt/src/memory.c \
	wolfssl/wolfcrypt/src/coding.c \
	wolfssl/wolfcrypt/src/hash.c \
	wolfssl/wolfcrypt/src/kdf.c \
	wolfssl/wolfcrypt/src/hmac.c \
	wolfssl/wolfcrypt/src/md5.c \
	wolfssl/wolfcrypt/src/wc_port.c \
	wolfssl/wolfcrypt/src/wc_encrypt.c \
	wolfssl/wolfcrypt/src/wolfmath.c \
	wolfssl/wolfcrypt/src/sp_c32.c \
	wolfssl/wolfcrypt/src/sp_int.c \
	wolfssl/wolfcrypt/src/signature.c \
	wolfssl/wolfcrypt/src/error.c \
	wolfssl/wolfcrypt/benchmark/benchmark.c \
	main.c

all: wolfbench

wolfbench: $(CSRCS)
	$(CC) $(CFLAGS) $^ -o wolfbench
