#include "fastpbkdf2.h"
#include <stdio.h>

// helper function that prints byte array into hex string
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