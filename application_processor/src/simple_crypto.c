#include "simple_crypto.h"
#include "ectf_params.h"
#include <stdint.h>
#include <string.h>
//Macros
#define SUCCESS_RETURN 0
#define ERROR_RETURN -1
// variable to store signature
uint8_t signature[SIGNATURE_SIZE];
word32 updated_sigSz;
ecc_key comm_key;
int pad_pkcs7(const char *data, int data_len, uint8_t *padded_data,
              int block_size) {
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

// void print_ecc_key(const char *label, ecc_key *key) {
//     byte buffer[1024]; // Buffer to hold exported key
//     word32 bufferSz = sizeof(buffer);
//     if (wc_ecc_export_x963(key, buffer, &bufferSz) != 0) {
//         printf("Error exporting %s key\n", label);
//         return;
//     }
//     printf("\n%s key: ", label);
//     for (int i = 0; i < bufferSz; i++) {
//         printf("%02X", buffer[i]);
//     }
//     printf("\n\n");
// }
int initialize_key() {
    int make_ret;
    int rand_ret;
    wc_ecc_init(&comm_key);
    WC_RNG rng;
    rand_ret = wc_InitRng(&rng);
    if(rand_ret!=0){
        return rand_ret;
    }
    make_ret  = wc_ecc_make_key(&rng, KEY_SIZE_, &comm_key);
    if (make_ret!= 0) {
        return make_ret;
    }
    return SUCCESS_RETURN;
    wc_FreeRng(&rng);
}
int sign(const uint8_t *data, size_t len, uint8_t *signature) {
    WC_RNG rng;
    wc_InitRng(&rng);
    word32 sigSz = SIGNATURE_SIZE;
    int key_ok=wc_ecc_check_key(&comm_key);
    if(key_ok != MP_OKAY){
        // print_error("Key not OK");
        return ERROR_RETURN;
    }
    int ret = wc_ecc_sign_hash(data, len, signature, &sigSz, &rng, &comm_key);
    updated_sigSz = sigSz;
    wc_FreeRng(&rng);
    if (ret != 0) {
        // print_error("Signing Failed");
        return ERROR_RETURN;
    }
    return SUCCESS_RETURN;
}
int sign_veriffy(const uint8_t *data, size_t len, uint8_t *signature){
    int result;
    int key_ok=wc_ecc_check_key(&comm_key);
    word32 sigSz = updated_sigSz;
    if (key_ok != MP_OKAY) return ERROR_RETURN;
    int ret = wc_ecc_verify_hash(signature, sigSz, data, len, &result, &comm_key);
    if (ret != 0)
        return ERROR_RETURN;
    return result;
}