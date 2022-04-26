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
static void char_to_bit_string(char c, char *out_str)
{
    int mask = 0x80; /* 10000000 */
    size_t i = 0;
    while (mask>0) {
        out_str[i] = (c & mask) > 0;
        i++;
        mask >>= 1; /* move the bit down */ 
    }
}

// static void 

// slices a string
// static void slice(const unsigned char *str, char *result, size_t start, size_t end)
// {
//     strncpy(result, str + start, end - start);
// }

// prints char array
static void print_char_array(unsigned char *buffer, size_t buffer_length) 
{
        printf("size of buffer: %ld\n", buffer_length);
        for(size_t i = 0; i < buffer_length; i++) {
                printf("%d", buffer[i]);
        }
        putchar('\n');
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
        // uint8_t *pw = "password";
        // size_t npw = 8;
        // uint8_t *salt = "salt";
        // size_t nsalt = 4;
        // uint32_t iterations = 4096;
        // uint8_t out[128];
        // size_t nout = sizeof(out);
        // fastpbkdf2_hmac_sha512(pw, npw, salt, nsalt, iterations, out, nout);
        // dump("got", out, nout);
        
        uint8_t random;
        unsigned char entropy[32];
        unsigned char entropy_str[256];
        random = generate_random_bytes(entropy, 32);
        unsigned char buffer[SHA256_DIGEST_LENGTH];
        size_t length = 32;
        *SHA256(entropy, length, buffer);
        for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i)
            printf("%02x", buffer[i]);
        putchar('\n');
        printf("length (hexadecimal): %d\n", SHA256_DIGEST_LENGTH);
        // unsigned char bit_str[8];
        // for(size_t i = 0; i < sizeof(entropy); i++) {
        //         char_to_bit_string(entropy[i], bit_str);
        //         for(size_t j = 0; j < sizeof(bit_str); j++) {
        //                 entropy_str[i*8 + j] = bit_str[j];
        //                 printf("i: %ld, j: %ld, i*8 + j: %ld\n", i, j, (i*8 + j) );
        //         }
        // }
        // // char_to_bit_string(entropy[0], bit_str);
        // print_char_array(entropy_str, sizeof(entropy_str));
        return 0;
}