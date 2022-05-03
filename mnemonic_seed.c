#include "fastpbkdf2.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <openssl/rand.h>
#include <openssl/sha.h>
#include "secp256k1.h"

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
                // print_char_array("temp", temp, 8);
                for(size_t j = 0; j < sizeof(temp); j++) {
                        mnemonic_bit_str[i*sizeof(temp) + j] = temp[j]; 
                }
        }
        // print_char_array("menmonic_bit_str", mnemonic_bit_str, 264);

        unsigned short int mnemonic_indicies[24];
        for(size_t i = 0; i < 24; i++) {
                unsigned char temp[11];
                unsigned short int tmp = 0;
                for(size_t j = 0; j < sizeof(temp); j++) {
                        temp[j] = mnemonic_bit_str[i*sizeof(temp) + j];
                }
                // print_char_array("mnemonic bit", temp, 11);
                for(size_t j = 0; j < sizeof(temp); j++) {
                        tmp = pow(2, j)*(temp[sizeof(temp) - j - 1] == 1) + tmp;
                }
                mnemonic_indicies[i] = tmp;
                // printf("mnemonic word number: %d\n", tmp);
        }

        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        unsigned char word_list[2048][9];



        fp = fopen("wordlist.txt", "r");
        if (fp == NULL)
                exit(EXIT_FAILURE);
        
        int n = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
                // printf("Retrieved line of length %zu:\n", read);
                line[strcspn(line, "\n")] = 0;   
                strcpy(word_list[n], line); 
                n++;
                
        }

        fclose(fp);
        if (line)
                free(line);

        size_t seed_length = strlen(word_list[mnemonic_indicies[0]]) + 1;
        unsigned char password[9*24];
        strcpy(password, strcat(word_list[mnemonic_indicies[0]], " "));

        for(size_t i = 1; i < sizeof(mnemonic_indicies)/sizeof(unsigned short int); i++) {
                if(i < sizeof(mnemonic_indicies)/sizeof(unsigned short int) - 1) {
                        strcat(word_list[mnemonic_indicies[i]], " ");
                }
                strcat(password, word_list[mnemonic_indicies[i]]);
                
                
                // printf("%s", word_list[mnemonic_indicies[i]]);
                // printf("%ld", strlen(word_list[mnemonic_indicies[i]]));
        }
        printf("seed phrase/password: %s\n", password);

        uint8_t *pw = password;
        size_t npw = strlen(password);
        uint8_t *salt = "mnemonic";
        size_t nsalt = 8;
        uint32_t iterations = 2048;
        uint8_t out[64];
        size_t nout = sizeof(out);
        fastpbkdf2_hmac_sha512(pw, npw, salt, nsalt, iterations, out, nout);
        dump("got", out, nout);

        int return_val;
        secp256k1_pubkey pubkey;
        secp256k1_ecdsa_signature sig;
        /* The specification in secp256k1.h states that `secp256k1_ec_pubkey_create` needs
        * a context object initialized for signing and `secp256k1_ecdsa_verify` needs
        * a context initialized for verification, which is why we create a context
        * for both signing and verification with the SECP256K1_CONTEXT_SIGN and
        * SECP256K1_CONTEXT_VERIFY flags. */
        secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);

        return 0;
}