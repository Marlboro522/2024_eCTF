// // // // #include <wolfssl/options.h>
// // // // #include <wolfssl/wolfcrypt/aes.h>
// // // // #include <wolfssl/wolfcrypt/random.h>
// // // // #include <stdio.h>
// // // // #include <stdlib.h>

// // // // #define AES_KEY_SIZE 32 
// // // // #define SUCCESS_RETURN 0
// // // // #define ERROR_RETURN -1

// // // // // Function to generate a random AES key
// // // // // int generate_aes_key(byte *aes_key) {
// // // // //     WC_RNG rng;
// // // // //     if (wc_InitRng(&rng) != 0) {
// // // // //         fprintf(stderr, "Error initializing RNG\n");
// // // // //         return ERROR_RETURN;
// // // // //     }
// // // // //     if (wc_RNG_GenerateBlock(&rng, aes_key, AES_KEY_SIZE) != 0) {
// // // // //         fprintf(stderr, "Error generating AES key\n");
// // // // //         wc_FreeRng(&rng);
// // // // //         return ERROR_RETURN;
// // // // //     }
// // // // //     wc_FreeRng(&rng);
// // // // //     return SUCCESS_RETURN;
// // // // // }

// // // // // // Function to encrypt data using AES-256
// // // // // int encrypt_data(const char *data, int data_len, uint8_t *encrypted_data, const uint8_t *key) {
// // // // //     Aes enc;
// // // // //     if (wc_AesSetKey(&enc, key, AES_KEY_SIZE, NULL, AES_ENCRYPTION) != 0) {
// // // // //         fprintf(stderr, "Error setting AES key\n");
// // // // //         return ERROR_RETURN;
// // // // //     }
// // // // //     if (wc_AesEncrypt(&enc, encrypted_data, (const uint8_t *)data) != 0) {
// // // // //         fprintf(stderr, "Error encrypting data\n");
// // // // //         return ERROR_RETURN;
// // // // //     }
// // // // //     return SUCCESS_RETURN;
// // // // // }

// // // // // // Function to decrypt data using AES-256
// // // // // int decrypt_data(const uint8_t *encrypted_data, int encrypted_data_len, char *decrypted_data, const uint8_t *key) {
// // // // //     Aes dec;
// // // // //     if (wc_AesSetKey(&dec, key, AES_KEY_SIZE, NULL, AES_DECRYPTION) != 0) {
// // // // //         fprintf(stderr, "Error setting AES key\n");
// // // // //         return ERROR_RETURN;
// // // // //     }
// // // // //     if (wc_AesDecrypt(&dec, (uint8_t *)decrypted_data, encrypted_data) != 0) {
// // // // //         fprintf(stderr, "Error decrypting data\n");
// // // // //         return ERROR_RETURN;
// // // // //     }
// // // // //     return SUCCESS_RETURN;
// // // // // }

// // // // // int main() {
// // // // //     byte aes_key[AES_KEY_SIZE];
    
// // // // //     // Generate AES key
// // // // //     if (generate_aes_key(aes_key) != SUCCESS_RETURN) {
// // // // //         fprintf(stderr, "Error generating AES key\n");
// // // // //         return EXIT_FAILURE;
// // // // //     }

// // // // //     // Example data to encrypt
// // // // //     const char *data_to_encrypt = "Hello, world!";
// // // // //     int data_len = strlen(data_to_encrypt);

// // // // //     // Buffer to hold encrypted data
// // // // //     uint8_t encrypted_data[data_len];

// // // // //     // Encrypt the data
// // // // //     if (encrypt_data(data_to_encrypt, data_len, encrypted_data, aes_key) != SUCCESS_RETURN) {
// // // // //         fprintf(stderr, "Error encrypting data\n");
// // // // //         return EXIT_FAILURE;
// // // // //     }

// // // // //     // Print the encrypted data
// // // // //     printf("Encrypted Data: ");
// // // // //     for (int i = 0; i < data_len; i++) {
// // // // //         printf("%02x", encrypted_data[i]);
// // // // //     }
// // // // //     printf("\n");

// // // // //     // Buffer to hold decrypted data
// // // // //     char decrypted_data[data_len];

// // // // //     // Decrypt the data
// // // // //     if (decrypt_data(encrypted_data, data_len, decrypted_data, aes_key) != SUCCESS_RETURN) {
// // // // //         fprintf(stderr, "Error decrypting data\n");
// // // // //         return EXIT_FAILURE;
// // // // //     }

// // // // //     // Print the decrypted data
// // // // //     printf("Decrypted Data: %s\n", decrypted_data);

// // // // //     return EXIT_SUCCESS;
// // // // // }
// // // #include <stdio.h>
// // // #include <stdint.h>
// // // #include <wolfssl/options.h>
// // // #include <wolfssl/wolfcrypt/random.h>

// // // #define AES_KEY_SIZE 32

// // // int main() {
// // //     byte aes_key[AES_KEY_SIZE];
// // //     WC_RNG rng;

// // //     /* Initialize the random number generator */
// // //     if (wc_InitRng(&rng) != 0) {
// // //         fprintf(stderr, "Error: Failed to initialize RNG\n");
// // //         return 1;
// // //     }

// // //     /* Generate a 256-bit AES key */
// // //     if (wc_RNG_GenerateBlock(&rng, aes_key, AES_KEY_SIZE) != 0) {
// // //         fprintf(stderr, "Error: Failed to generate AES key\n");
// // //         return 1;
// // //     }

// // //     /* Print the AES key in hexadecimal format */
// // //     printf("AES Key: ");
// // //     for (int i = 0; i < AES_KEY_SIZE; ++i) {
// // //         printf("%02x", aes_key[i]);
// // //     }
// // //     printf("\n");

// // //     /* Clean up the random number generator */
// // //     wc_FreeRng(&rng);

// // //     return 0;
// // // }
// // #include <stdbool.h>
// // #include <stdint.h>
// // #include <stdio.h>
// // #include <string.h>
// // #include "application_processor/inc/simple_crypto.h"
// // #include <wolfssl/options.h>
// // #include <wolfssl/wolfcrypt/aes.h>
// // #include <wolfssl/wolfcrypt/random.h>
// // #include "ectf_params.h"
// // #include <stdint.h>
// // #include <string.h>
// // //Macros
// // #define ERROR -1
// // #define SUCCESS 0
// //   //Fucntion definitions
// // void generate_key(byte *key) { WC_RNG rng;
// //     wc_InitRng(&rng);
// //     wc_RNG_GenerateBlock(&rng, key, KEY_SIZE);
// //     wc_FreeRng(&rng);
// // }

// // int encrypt(const char *pin, int pin_len, uint8_t *encrypted_pin, const uint8_t *key, const uint8_t *iv) {
// //     Aes enc;
// //     if (wc_AesSetKey(&enc, key, KEY_SIZE, iv, AES_ENCRYPTION) != 0) {
// //         return ERROR;
// //     }
// //     if (wc_AesCbcEncrypt(&enc, encrypted_pin, (const uint8_t *)pin, pin_len) != 0) {
// //         return ERROR;
// //     }
// //     return SUCCESS;
// // }

// // int compare_pins(const uint8_t *encrypted_pin, const uint8_t *encrypted_pin1){
// //     for (int i = 0; i < KEY_SIZE;i++){
// //         if(encrypted_pin[i] != encrypted_pin1[i]){
// //             return ERROR;
// //         }
// //         return SUCCESS;
// //     }
// // }

// // #define AES_KEY_SIZE 32
// // #define AP_PIN "1234567"
// // byte *key;
// //     uint8_t o_CIPHER;
// //     uint8_t u_CIPHER;
// //     uint8_t iv[BLOCK_SIZE];
// //     memset(iv, 0x00, BLOCK_SIZE);
// //     generate_key(key);
// //     encrypt(AP_PIN,strlen(AP_PIN),o_CIPHER,key,iv);
// //     char user_PIN[50];
// //     uint8_t encryted_PIN[BLOCK_SIZE];
// //     printf("Enter PIN: ");
// //     scanf("%s",&user_PIN);
// //     if(encrypt(user_PIN,strlen(user_PIN),u_CIPHER,key,iv)!=0){
// //         return ERROR_RETURN;
// //     }
// //     if(compare_pins(o_CIPHER,u_CIPHER)==SUCCESS_RETURN){
// //         print_debug("PIN ACCEPTED!\n");
// //         return SUCCESS_RETURN;
// //     } else {
// //         print_error("Invalid PIN!\n");
// //         return ERROR_RETURN;
// //     }
// // // int main() {
// // //     byte aes_key[AES_KEY_SIZE];
// // //     byte iv[AES_BLOCK_SIZE];
// // //     byte ciphertext[256]; // Adjust size as needed
// // //     byte decryptedtext[256]; // Adjust size as needed

// // //     WC_RNG rng;

// // //     /* Initialize the random number generator */
// // //     if (wc_InitRng(&rng) != 0) {
// // //         fprintf(stderr, "Error: Failed to initialize RNG\n");
// // //         return 1;
// // //     }

// // //     /* Generate a 256-bit AES key */
// // //     if (wc_RNG_GenerateBlock(&rng, aes_key, AES_KEY_SIZE) != 0) {
// // //         fprintf(stderr, "Error: Failed to generate AES key\n");
// // //         return 1;
// // //     }

// // //     /* Print the AES key in hexadecimal format */
// // //     printf("AES Key: ");
// // //     for (int i = 0; i < AES_KEY_SIZE; ++i) {
// // //         printf("%02x", aes_key[i]);
// // //     }
// // //     printf("\n");

// // //     /* Generate a random IV */
// // //     if (wc_RNG_GenerateBlock(&rng, iv, AES_BLOCK_SIZE) != 0) {
// // //         fprintf(stderr, "Error: Failed to generate IV\n");
// // //         return 1;
// // //     }

// // //     /* Initialize AES encryption context */
// // //     Aes enc;
// // //     if (wc_AesSetKey(&enc, aes_key, AES_KEY_SIZE, iv, AES_ENCRYPTION) != 0) {
// // //         fprintf(stderr, "Error: Failed to initialize AES encryption context\n");
// // //         return 1;
// // //     }

// // //     /* Encrypt plaintext */
// // //     int plaintext_len = strlen(PLAINTEXT);
// // //     if (wc_AesCbcEncrypt(&enc, ciphertext, (const byte*)PLAINTEXT, plaintext_len) != 0) {
// // //         fprintf(stderr, "Error: Failed to encrypt plaintext\n");
// // //         return 1;
// // //     }

// // //     /* Print the ciphertext in hexadecimal format */
// // //     printf("Ciphertext: ");
// // //     for (int i = 0; i < plaintext_len; ++i) {
// // //         printf("%02x", ciphertext[i]);
// // //     }
// // //     printf("\n");

// // //     /* Decrypt ciphertext */
// // //     Aes dec;
// // //     if (wc_AesSetKey(&dec, aes_key, AES_KEY_SIZE, iv, AES_DECRYPTION) != 0) {
// // //         fprintf(stderr, "Error: Failed to initialize AES decryption context\n");
// // //         return 1;
// // //     }
// // //     if (wc_AesCbcDecrypt(&dec, decryptedtext, ciphertext, plaintext_len) != 0) {
// // //         fprintf(stderr, "Error: Failed to decrypt ciphertext\n");
// // //         return 1;
// // //     }

// // //     /* Null-terminate decrypted text */
// // //     decryptedtext[plaintext_len] = '\0';

// // //     /* Print the decrypted plaintext */
// // //     printf("Decrypted Plaintext: %s\n", decryptedtext);

// // //     return 0;
// // // }
// #include <stdio.h>
// #include <string.h>
// #include <wolfssl/wolfcrypt/aes.h>
// #include <wolfssl/wolfcrypt/random.h>

// #define KEY_SIZE 32
// #define BLOCK_SIZE 32
// #define SUCCESS_RETURN 0
// #define ERROR_RETURN -1
// #define SALT_LEN 13

// void gen_salt(char *salt){
//     // uint8_t salt[SALT_LEN];
//     WC_RNG rng;

//     if (wc_InitRng(&rng) != 0) {
//         return ; // Failed for random number generator
//     }

//     // Generate random salt
//     if (wc_RNG_GenerateBlock(&rng, salt, SALT_LEN) != 0) {
//         wc_FreeRng(&rng);
//         return ; // Failed to gen salt
//     }

//     wc_FreeRng(&rng);
// }

// int pad_pkcs7(const char *data, int data_len, uint8_t *padded_data, int block_size) {
//     int padded_len = block_size * ((data_len + block_size - 1) / block_size); // Calculate the padded length
//     int padding_bytes = padded_len - data_len; // Calculate the number of padding bytes to add
//     memcpy(padded_data, data, data_len); // Copy the original data
//     memset(padded_data + data_len, padding_bytes, padding_bytes); // Add padding bytes
//     return padded_len; // Return the padded length
// }
// // Function to generate a random key
// int generate_key(uint8_t *key) {
//     WC_RNG rng;
//     if (wc_InitRng(&rng) != 0) {
//         return ERROR_RETURN; // Failed to initialize random number generator
//     }
//     if (wc_RNG_GenerateBlock(&rng, key, KEY_SIZE) != 0) {
//         wc_FreeRng(&rng);
//         return ERROR_RETURN; // Failed to generate random key
//     }
//     wc_FreeRng(&rng);
//     return SUCCESS_RETURN;
// }

// // Function to generate a random initialization vector (IV)
// int generate_random_iv(uint8_t *iv) {
//     WC_RNG rng;
//     if (wc_InitRng(&rng) != 0) {
//         return ERROR_RETURN; // Failed to initialize random number generator
//     }
//     if (wc_RNG_GenerateBlock(&rng, iv, KEY_SIZE) != 0) { 
//         wc_FreeRng(&rng);
//         return ERROR_RETURN; // Failed to generate random IV
//     }
//     wc_FreeRng(&rng);
//     return SUCCESS_RETURN;
// }

// // Function to encrypt data using AES-256 in CBC mode
// int encrypt_n(const char *pin, int pin_len, uint8_t *encrypted_pin, const uint8_t *key, uint8_t *iv) {
//     uint8_t padded_pin[BLOCK_SIZE];
//     // uint8_t *salt;
//     // get salt
//     // gen_salt(salt);
//     // Concatenate pin and salt
//     // char pin_with_salt[strlen(pin) + SALT_LEN + 1]; // +1 for null terminator
//     // strcpy(pin_with_salt, pin);
//     // strcat(pin_with_salt, (char*)salt);
//     // printf(pin_with_salt);

//     // Pad the concatenated pin with PKCS#7
//     int padded_len = pad_pkcs7(pin, strlen(pin), padded_pin, BLOCK_SIZE);

//     // Encrypt padded pin
//     Aes enc;
//     if (wc_AesSetKey(&enc, key, KEY_SIZE, iv, AES_ENCRYPTION) != 0) {
//         return ERROR_RETURN; // Failed to set AES key
//     }
//     if (wc_AesCbcEncrypt(&enc, encrypted_pin, padded_pin, padded_len) != 0) {
//         return ERROR_RETURN; // Failed to encrypt pin
//     }
//     return SUCCESS_RETURN;
// }

// // Function to compare two encrypted PINs
// int compare_pins(const uint8_t *encrypted_pin1, const uint8_t *encrypted_pin2) {
//     // Compare the encrypted PINs byte by byte
//     for (int i = 0; i < BLOCK_SIZE; i++) {
//         if (encrypted_pin1[i] != encrypted_pin2[i]) {
//             return ERROR_RETURN; // PINs don't match
//         }
//     }
//     return SUCCESS_RETURN; // PINs match
// }

// void bytes_to_hex(const uint8_t *bytes, int len, char *hex_str) {
//     for (int i = 0; i < len; i++) {
//         sprintf(hex_str + (i * 2), "%02x", bytes[i]);
//     }
// }


// #define AP_PIN "76ea8r"

// int main() {
//     uint8_t key[KEY_SIZE];
//     uint8_t o_CIPHER[BLOCK_SIZE];
//     uint8_t u_CIPHER[BLOCK_SIZE];
//     uint8_t iv[KEY_SIZE];
//     char hex_str[BLOCK_SIZE * 2 + 1];
//     uint8_t *salt;
//     generate_key(key);
//     generate_random_iv(iv);
//     bytes_to_hex(key,KEY_SIZE,hex_str);
//     gen_salt(salt);
//     printf("AP_PIN: %s", AP_PIN);
//     strcat(AP_PIN, (char *)salt);
//     printf("\n");
//     printf("AP_PIN: %s", AP_PIN);
//     printf("\n");
//     // Encrypt original PIN
//     encrypt_n(AP_PIN, strlen(AP_PIN) + 1, o_CIPHER, key, iv); //wtf is this fine and application is not
//     // Print original PIN cipher text
//     printf("Original PIN (o_CIPHER): ");
//     bytes_to_hex(o_CIPHER, BLOCK_SIZE, hex_str);
//     printf("%s\n\n", hex_str);

//     // Get user PIN
//     char user_PIN[50]; 
//     printf("Enter PIN: ");
//     scanf("%s", user_PIN);
//     printf("\n");
//     strcat(user_PIN, (char *)salt);
//     scanf("%s", user_PIN);
//     printf("\n");
//     // Encrypt user PIN
//     if (encrypt_n(user_PIN, strlen(user_PIN) + 1, u_CIPHER, key, iv) != 0) {
//         return ERROR_RETURN;
//     }
//     printf("%s\n", AP_PIN);
//     // Print user PIN cipher text
//     printf("Encrypted User PIN (u_CIPHER): ");
//     bytes_to_hex(u_CIPHER, BLOCK_SIZE, hex_str);
//     printf("%s\n", hex_str);

//     // Compare
//     if (compare_pins(o_CIPHER, u_CIPHER) == SUCCESS_RETURN) {
//         printf("PIN ACCEPTED!\n");
//         return SUCCESS_RETURN;
//     } else {
//         printf("Invalid PIN!!!\n");
//         return ERROR_RETURN;
//     }
// }
#include <stdio.h>
#include <string.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/random.h>

#define KEY_SIZE 32
#define BLOCK_SIZE 32
#define SUCCESS_RETURN 0
#define ERROR_RETURN -1
#define SALT_LEN 13

void gen_salt(char *salt){
    WC_RNG rng;

    wc_InitRng(&rng);
    // Generate random salt
    wc_RNG_GenerateBlock(&rng, (uint8_t *)salt, SALT_LEN);

    wc_FreeRng(&rng);
}

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

    // Pad the concatenated pin with PKCS#7
    int padded_len = pad_pkcs7(pin, pin_len, padded_pin, BLOCK_SIZE);

    // Encrypt padded pin
    Aes enc;
    if (wc_AesSetKey(&enc, key, KEY_SIZE, iv, AES_ENCRYPTION) != 0) {
        return ERROR_RETURN; // Failed to set AES key
    }
    if (wc_AesCbcEncrypt(&enc, encrypted_pin, padded_pin, padded_len) != 0) {
        return ERROR_RETURN; // Failed to encrypt pin
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

#define AP_PIN "76ea8r"

int main() {

    uint8_t key[KEY_SIZE];
    uint8_t o_CIPHER[BLOCK_SIZE];
    uint8_t u_CIPHER[BLOCK_SIZE];
    uint8_t iv[KEY_SIZE];
    char hex_str[BLOCK_SIZE * 2 + 1];
    uint8_t salt[SALT_LEN+1]; // +1 for null terminator
    char new_p[21];

    generate_key(key);
    generate_random_iv(iv);
    bytes_to_hex(key, KEY_SIZE, hex_str);
    gen_salt((char *)salt);
    printf("AP_PIN: %s\n", AP_PIN);
    strcpy(new_p, AP_PIN);    
    strcat(new_p,(char *) salt);
    printf("\n");
    printf("AP_PIN with salt: %s\n", new_p);
    printf("\n");
    printf("Length of AP_PIN with salt: %zu\n", strlen(new_p));
    // Encrypt original PIN
    encrypt_n(new_p, strlen(new_p), o_CIPHER, key, iv);
    // Print original PIN cipher text
    printf("Original PIN (o_CIPHER): ");
    bytes_to_hex(o_CIPHER, BLOCK_SIZE, hex_str);
    printf("%s\n\n", hex_str);
    printf("LENGTH OF o_CIPHER: %zu\n", strlen(hex_str));
    // Get user PIN
    memset(new_p, 0, 21);
    char user_PIN[50];
    printf("\nEnter PIN: ");
    scanf("%s", user_PIN);
    printf("\n");
    strcpy(new_p, user_PIN);
    strcat(new_p, (char *)salt);
    printf("Length of encrypt data: %zu\n", strlen(new_p));
    // printf("%s\n", typeof(salt));
    printf("\n");
    printf("Userpin with salt: %s\n\n", new_p);
    // Encrypt user PIN
    if (encrypt_n(new_p, strlen(new_p), u_CIPHER, key, iv) != 0) {
        return ERROR_RETURN;
    }
    printf("%s\n\n", AP_PIN);
    // Print user PIN cipher text
    printf("Encrypted User PIN (u_CIPHER): ");
    bytes_to_hex(u_CIPHER, BLOCK_SIZE, hex_str);
    printf("%s\n\n", hex_str);
    printf("Length of u_CIPHER: %zu\n\n", strlen(hex_str));

    // Compare
    if (compare_pins(o_CIPHER, u_CIPHER) == SUCCESS_RETURN) {
        printf("PIN ACCEPTED!\n\n");
        return SUCCESS_RETURN;
    } else {
        printf("Invalid PIN!!!\n\n");
        return ERROR_RETURN;
    }
}




