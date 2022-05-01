#include "fastpbkdf2.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

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

// prints char array
static void print_char_array(const char *label, unsigned char *buffer, size_t buffer_length) 
{
        printf("%s: ", label);
        for(size_t i = 0; i < buffer_length; i++) {
                printf("%d", buffer[i]);
        }
        putchar('\n');
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



// prints byte array into hex string
static void dump(const char *label, const uint8_t *data, size_t n)
{
  printf("%s: ", label);
  for (size_t i = 0; i < n; i++)
    printf("%02x", data[i]);
  printf("\n");
}

int fromBinary(const char *s) {
  return (int) strtol(s, NULL, 2);
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
        dump("entropy", entropy, 32);
        *SHA256(entropy, length, buffer);
        
        unsigned char mnemonic[33];
        memcpy(mnemonic, entropy, sizeof(mnemonic));
        mnemonic[33] = buffer[0];
        dump("sha256", buffer, 32);
        dump("mnemonic", mnemonic, 33);
        
        unsigned char mnemonic_bit_str[264];
        for(size_t i = 0; i < sizeof(mnemonic); i++) {
                unsigned char temp[8];
                char_to_bit_string(mnemonic[i], temp);
                print_char_array("temp", temp, 8);
                for(size_t j = 0; j < sizeof(temp); j++) {
                        mnemonic_bit_str[i*sizeof(temp) + j] = temp[j]; 
                }
        }
        print_char_array("menmonic_bit_str", mnemonic_bit_str, 264);

        unsigned short int menmonic_bit_chunks[24];
        for(size_t i = 0; i < 24; i++) {
                unsigned char temp[11];
                unsigned short int tmp = 0;
                for(size_t j = 0; j < sizeof(temp); j++) {
                        temp[j] = mnemonic_bit_str[i*sizeof(temp) + j];
                }
                print_char_array("mnemonic bit", temp, 11);
                for(size_t j = 0; j < sizeof(temp); j++) {
                        tmp = pow(2, j)*(temp[sizeof(temp) - j - 1] == 1) + tmp;
                }
                menmonic_bit_chunks[i] = tmp;
                printf("mnemonic word number: %d\n", tmp);
        }

        

        return 0;
}