/**
 * @file "simple_crypto.c"
 * @author Ben Janis
 * @brief Simplified Crypto API Implementation
 * @date 2024
 *
 * This source file is part of an example system for MITRE's 2024 Embedded System CTF (eCTF).
 * This code is being provided only for educational purposes for the 2024 MITRE eCTF competition,
 * and may not meet MITRE standards for quality. Use this code at your own risk!
 *
 * @copyright Copyright (c) 2024 The MITRE Corporation
 */

#if CRYPTO_EXAMPLE

#include "simple_crypto.h"
#include <stdint.h>
#include <string.h>

/******************************** FUNCTION PROTOTYPES ********************************/
/** @brief Encrypts plaintext using a symmetric cipher
 *
 * @param plaintext A pointer to a buffer of length len containing the
 *          plaintext to encrypt
 * @param len The length of the plaintext to encrypt. Must be a multiple of
 *          BLOCK_SIZE (16 bytes)
 * @param key A pointer to a buffer of length KEY_SIZE (16 bytes) containing
 *          the key to use for encryption
 * @param ciphertext A pointer to a buffer of length len where the resulting
 *          ciphertext will be written to
 *
 * @return 0 on success, -1 on bad length, other non-zero for other error
 */
int encrypt_sym(uint8_t *plaintext, size_t len, uint8_t *key, uint8_t *ciphertext) {
    Aes ctx; // Context for encryption
    int result; // Library result

    // Ensure valid length
    if (len <= 0 || len % BLOCK_SIZE)
        return -1;

    // Set the key for encryption
    result = wc_AesSetKey(&ctx, key, 16, NULL, AES_ENCRYPTION);
    if (result != 0)
        return result; // Report error


    // Encrypt each block
    for (int i = 0; i < len - 1; i += BLOCK_SIZE) {
        result = wc_AesEncryptDirect(&ctx, ciphertext + i, plaintext + i);
        if (result != 0)
            return result; // Report error
    }
    return 0;
}

/** @brief Decrypts ciphertext using a symmetric cipher
 *
 * @param ciphertext A pointer to a buffer of length len containing the
 *          ciphertext to decrypt
 * @param len The length of the ciphertext to decrypt. Must be a multiple of
 *          BLOCK_SIZE (16 bytes)
 * @param key A pointer to a buffer of length KEY_SIZE (16 bytes) containing
 *          the key to use for decryption
 * @param plaintext A pointer to a buffer of length len where the resulting
 *          plaintext will be written to
 *
 * @return 0 on success, -1 on bad length, other non-zero for other error
 */
int decrypt_sym(uint8_t *ciphertext, size_t len, uint8_t *key, uint8_t *plaintext) {
    Aes ctx; // Context for decryption
    int result; // Library result

    // Ensure valid length
    if (len <= 0 || len % BLOCK_SIZE)
        return -1;

    // Set the key for decryption
    result = wc_AesSetKey(&ctx, key, 16, NULL, AES_DECRYPTION);
    if (result != 0)
        return result; // Report error

    // Decrypt each block
    for (int i = 0; i < len - 1; i += BLOCK_SIZE) {
        result = wc_AesDecryptDirect(&ctx, plaintext + i, ciphertext + i);
        if (result != 0)
            return result; // Report error
    }
    return 0;
}

/** @brief Hashes arbitrary-length data
 *
 * @param data A pointer to a buffer of length len containing the data
 *          to be hashed
 * @param len The length of the plaintext to encrypt
 * @param hash_out A pointer to a buffer of length HASH_SIZE (16 bytes) where the resulting
 *          hash output will be written to
 *
 * @return 0 on success, non-zero for other error
 */
int hash(void *data, size_t len, uint8_t *hash_out) {
    // Pass values to hash
    return wc_Md5Hash((uint8_t *)data, len, hash_out);
}

#endif
#include "simple_crypto.h"
#include "ectf_params.h"
#include <stdint.h>
#include <string.h>
//Macros
#define SUCCESS_RETURN 0
#define ERROR_RETURN -1

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

// void bytes_to_hex(const uint8_t *bytes, int len, char *hex_str) {
//     for (int i = 0; i < len; i++) {
//         sprintf(hex_str + (i * 2), "%02x", bytes[i]);
//     }
// }
