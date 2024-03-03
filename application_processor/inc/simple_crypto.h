#include "wolfssl/wolfcrypt/aes.h"
#include "wolfssl/wolfcrypt/hash.h"
#include "wolfssl/wolfcrypt/sha256.h"
#include<wolfssl/wolfcrypt/aes.h>
#include<wolfssl/wolfcrypt/random.h>

#define KEY_SIZE 32
#define BLOCK_SIZE 16
#define SUCCESS_RETURN 0
#define ERROR_RETURN -1
//Fucntion prototypes
int pad_pkcs7(const char *data, int data_len, uint8_t *padded_data,
              int block_size);

int generate_key(uint8_t *key);

int generate_random_iv(uint8_t *iv);

int encrypt_n(const char *pin, int pin_len, uint8_t *encrypted_pin,
             const uint8_t *key, uint8_t *iv);

int compare_pins(const uint8_t *encrypted_pin, const uint8_t *encrypted_pin1);

void bytes_to_hex(const uint8_t *bytes, int len, char *hex_str);

void hash_pin(const char* pin, uint8_t* hash);

void enroll_pin(const char *pin);
