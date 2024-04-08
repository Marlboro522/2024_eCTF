#include<wolfssl/options.h>
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/random.h>
#include<wolfssl/wolfcrypt/ecc.h>
#include <wolfssl/wolfcrypt/asn.h>
#include <wolfssl/wolfcrypt/types.h>
// #include "../../msdk/Libraries/PeriphDrivers/Include/MAX78000/trng.h"
//Defence
#define KEY_SIZE 32
#define BLOCK_SIZE 16
#define SUCCESS_RETURN 0
#define ERROR_RETURN -1
#define SALT_LEN 13
//Comms
#define KEY_SIZE_ 16
#define SIGNATURE_SIZE 64
extern ecc_key comm_key;
// Fucntion prototypes
int pad_pkcs7(const char *data, int data_len, uint8_t *padded_data,
              int block_size);

int generate_key(uint8_t *key);

int generate_random_iv(uint8_t *iv);

int encrypt_n(const char *pin, int pin_len, uint8_t *encrypted_pin,
             const uint8_t *key, uint8_t *iv);

int compare_pins(const uint8_t *encrypted_pin, const uint8_t *encrypted_pin1);

void gen_salt(char *salt);

void bytes_to_hex(const uint8_t *bytes, int len, char *hex_str);

int initialize_key();

int sign(const uint8_t *data, size_t len, uint8_t *signature);

int sign_veriffy(const uint8_t *data, size_t len, uint8_t *signature);

// void hash_pin(const char* pin, uint8_t* hash);

// void enroll_pin(const char *pin);
