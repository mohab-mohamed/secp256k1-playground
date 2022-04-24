#include <openssl/sha.h>
#include <stdio.h>

// $ gcc -o sha256 sha256.c -L/usr/lib -lcrypto


int main(int argc, char **argv)
 {
    unsigned char buffer[SHA256_DIGEST_LENGTH];
    unsigned char *data = "password";
    size_t length = 8;
    *SHA256(data, length, buffer);
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i)
            printf("%02x", buffer[i]);
    putchar('\n');
    printf("length (hexadecimal): %d\n", SHA256_DIGEST_LENGTH);
    return 0;
 }