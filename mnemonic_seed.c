#include "fastpbkdf2.h"
#include <stdio.h>

#include <openssl/rand.h>
#include <openssl/sha.h>

// generates random number of bytes 
static int generate_random_bytes(unsigned char *buf, int num)
{
        return RAND_priv_bytes(buf, num);
}

// implements sha256
static hash_via_sha256(unsigned char *buf, SHA256_CTX ctx, size_t length) 
{
        SHA256_Init(&ctx);
        do {
                SHA256_Update(&ctx, buffer, len);
        } while (len == BUFSIZ);

        SHA256_Final(buffer, &ctx);
}


// appends checksum 
// static void append_checksum

// implements pbkdf2
static void seed_via_pbkdf2(uint8_t *password, size_t password_lentgh, 
                            uint8_t *salt, size_t salt_length, 
                            uint32_t iterations, uint8_t *out, size_t out_length)
{
        fastpbkdf2_hmac_sha512(pw, npw, salt, nsalt, iterations, out, nout);
}



// prints byte array into hex string
static void dump(const char *label, const uint8_t *data, size_t n)
{
  printf("%s: ", label);
  for (size_t i = 0; i < n; i++)
    printf("%02x", data[i]);
  printf("\n");
}

int main(int argc, char **argv)
{
        uint8_t *pw = "password";
        size_t npw = 8;
        uint8_t *salt = "salt";
        size_t nsalt = 4;
        uint32_t iterations = 4096;
        uint8_t out[128];
        size_t nout = sizeof(out);
        fastpbkdf2_hmac_sha512(pw, npw, salt, nsalt, iterations, out, nout);
        dump("got", out, nout);
        return 0;
}