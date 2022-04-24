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
static void hash_via_sha256(unsigned char *data, size_t length, unsigned char *buffer) 
{
        *SHA256(data, length, buffer);
}


// appends checksum 
// static void append_checksum

// implements pbkdf2
static void seed_via_pbkdf2(uint8_t *password, size_t password_length, 
                            uint8_t *salt, size_t salt_length, 
                            uint32_t iterations, uint8_t *out, size_t out_length)
{
        fastpbkdf2_hmac_sha512(password, password_length, salt, salt_length, iterations, out, out_length);
}

// ---------------helper functions ------------------------------

// convert from a byte array to bit string
static void byte_array_to_bit_string(unsigned char *byte, unsigned char *buffer) {
        // for(size_t i=0; i < sizeof(byte))
        // sprintf(buffer, "%08b", byte[0])
}

// char to bit string
void printbincharpad(char c)
{
    for (int i = 7; i >= 0; --i)
    {
        putchar( (c & (1 << i)) ? '1' : '0' );
    }
    putchar('\n');
}

// slices a string
// static void slice(const unsigned char *str, char *result, size_t start, size_t end)
// {
//     strncpy(result, str + start, end - start);
// }

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
        // uint8_t *pw = "password";
        // size_t npw = 8;
        // uint8_t *salt = "salt";
        // size_t nsalt = 4;
        // uint32_t iterations = 4096;
        // uint8_t out[128];
        // size_t nout = sizeof(out);
        // fastpbkdf2_hmac_sha512(pw, npw, salt, nsalt, iterations, out, nout);
        // dump("got", out, nout);
        
        unsigned char entropy[256];
        generate_random_bytes(entropy, 256);
        for(size_t i = 0; i < sizeof(entropy); i++) {
                printbincharpad(entropy[i]);
        }
        printf("%ld", sizeof(entropy));
        // printf("%s\n", entropy);
        return 0;
}