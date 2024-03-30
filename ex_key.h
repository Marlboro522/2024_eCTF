#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/ecc.h>
#include <stdio.h>
#include <stdlib.h>

#define KEY_SIZE_ 16
#define SIGNATURE_SIZE 64
extern ecc_key comm_key;
#define SUCCESS_RETURN 0
#define ERROR_RETURN -1
int initialize_key();

int sign(const uint8_t *data, size_t len, uint8_t *signature);

int sign_veriffy(const uint8_t *data, size_t len, uint8_t *signature);