#include "simple_crypto.h"
#include "ectf_params.h"
#include <stdint.h>
#include <string.h>
//Macros
#define SUCCESS_RETURN 0
#define ERROR_RETURN -1
// #define FLASH_ADDRESS 0x0800FC00

int pad_pkcs7(const char *data, int data_len, uint8_t *padded_data, int block_size) {
    int padded_len = block_size * ((data_len + block_size - 1) / block_size); // Calculate the padded length
    int padding_bytes = padded_len - data_len; // Calculate the number of padding bytes to add
    memcpy(padded_data, data, data_len); // Copy the original data
    memset(padded_data + data_len, padding_bytes, padding_bytes); // Add padding bytes
    return padded_len; // Return the padded length
}
// Function to generate a random key
int generate_key(uint8_t *key) {
    WC_RNG rng;
    if (wc_InitRng(&rng) != 0) {
        return ERROR_RETURN; // Failed to initialize random number generator
    }
    if (wc_RNG_GenerateBlock(&rng, key, KEY_SIZE) != 0) {
        wc_FreeRng(&rng);
        return ERROR_RETURN; // Failed to generate random key
    }
    wc_FreeRng(&rng);
    return SUCCESS_RETURN;
}

// Function to generate a random initialization vector (IV)
int generate_random_iv(uint8_t *iv) {
    WC_RNG rng;
    if (wc_InitRng(&rng) != 0) {
        return ERROR_RETURN; // Failed to initialize random number generator
    }
    if (wc_RNG_GenerateBlock(&rng, iv, KEY_SIZE) != 0) { 
        wc_FreeRng(&rng);
        return ERROR_RETURN; // Failed to generate random IV
    }
    wc_FreeRng(&rng);
    return SUCCESS_RETURN;
}

// Function to encrypt data using AES-256 in CBC mode
int encrypt_n(const char *pin, int pin_len, uint8_t *encrypted_pin, const uint8_t *key, uint8_t *iv) {
    uint8_t padded_pin[BLOCK_SIZE];
    int padded_len = pad_pkcs7(pin, pin_len, padded_pin, BLOCK_SIZE);
    Aes enc;
    if (wc_AesSetKey(&enc, key, KEY_SIZE, iv, AES_ENCRYPTION) != 0) {
        return ERROR_RETURN;
    }
    if (wc_AesCbcEncrypt(&enc, encrypted_pin, padded_pin, padded_len) != 0) {
        return ERROR_RETURN;
    }
    return SUCCESS_RETURN;
}

// Function to compare two encrypted PINs
int compare_pins(const uint8_t *encrypted_pin1, const uint8_t *encrypted_pin2) {
    // Compare the encrypted PINs byte by byte
    for (int i = 0; i < BLOCK_SIZE; i++) {
        if (encrypted_pin1[i] != encrypted_pin2[i]) {
            return ERROR_RETURN; // PINs don't match
        }
    }
    return SUCCESS_RETURN; // PINs match
}

void bytes_to_hex(const uint8_t *bytes, int len, char *hex_str) {
    for (int i = 0; i < len; i++) {
        sprintf(hex_str + (i * 2), "%02x", bytes[i]);
    }
}

void gen_salt(char *salt){
    WC_RNG rng;

    wc_InitRng(&rng);
    // Generate random salt
    wc_RNG_GenerateBlock(&rng, (uint8_t *)salt, SALT_LEN);

    wc_FreeRng(&rng);
}


// void enroll_pin(const char* pin) {
//     uint8_t hash[SHA256_DIGEST_SIZE];
//     uint8_t encrypted_hash[AES_BLOCK_SIZE];

//     hash_pin(pin, hash);
//     encrypt_pin(hash, encrypted_hash);

//     flash_write(encrypted_hash, AES_BLOCK_SIZE, FLASH_ADDRESS);
// }

