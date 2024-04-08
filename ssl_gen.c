// // // // #include <wolfssl/options.h>
// // // // // // #include <wolfssl/ssl.h>
// // // // #include <wolfssl/wolfcrypt/ecc.h>
// // // #include <stdio.h>
// // // // // #include "msdk/Libraries/PeriphDrivers/Include/MAX78000/trng.h"
// // // // #include<wolfssl/wolfcrypt/random.h>
// // // #include <stdlib.h>
// // // #include <stdint.h>
// // // #include <mxc_device.h>
// // // #include <nvic_table.h>
// // // #include <trng.h>
// // // #include "ssl_gen.h"

// // // #define KEY_SIZE_ 16 // 32 bytes for 128-bit keys
// // // #define SIGNATURE_SIZE 64 // 64 bytes for 512-bit ECDSA signatures
// // // #define EXIT_FAILURE 1
// // // #define EXIT_SUCCESS 0
// // // word32 updated_sigSz;
// // // // Global variables for storing keys
// // // ecc_key key;

// // // void print_ecc_key(const char *label, ecc_key *key1) {
// // //     byte buffer[1024]; // Buffer to hold exported key
// // //     word32 bufferSz = sizeof(buffer);
// // //     if (wc_ecc_export_x963(key1, buffer, &bufferSz) != 0) {
// // //         printf("Error exporting %s key\n", label);
// // //         return;
// // //     }
// // //     printf("\n%s key: ", label);
// // //     for (int i = 0; i < bufferSz; i++) {
// // //         printf("%02X", buffer[i]);
// // //     }
// // //     printf("\n\n");
// // // }
// // // int initialize_key() {
// // //     volatile int wait;
// // //     volatile int callback_result;
// // //     /***** Globals *****/
// // //     uint8_t var_rnd_no[16] = { 0 };
// // //     void TRNG_IRQHandler(void){
// // //         MXC_TRNG_Handler();
// // //     }
// // //     void Test_Callback(void *req, int result){
// // //         wait = 0;
// // //         callback_result = result;
// // //     }
// // //     void Test_TRNG(int asynchronous){
// // //         printf(asynchronous ? "\nTest TRNG Async\n" : "\nTest TRNG Sync\n");
// // //         int num_bytes = 16;
// // //         memset(var_rnd_no, 0, sizeof(var_rnd_no));
// // //         MXC_TRNG_Init();
// // //         if (asynchronous) {
// // //             wait = 1;
// // //             NVIC_EnableIRQ(TRNG_IRQn);
// // //             MXC_TRNG_RandomAsync(var_rnd_no, num_bytes, &Test_Callback);
// // //             while (wait) {}
// // //         } else {
// // //             MXC_TRNG_Random(var_rnd_no, num_bytes);
// // //         } MXC_TRNG_Shutdown();
// // //     }
// // //     wc_ecc_init(&key);
// // //     WC_RNG rng;
// // //     wc_FreeRng(&rng);
// // //     wc_InitRng(&rng);
// // //     if (wc_ecc_make_key(&rng, KEY_SIZE_, &key) != 0) {
// // //         printf("\nError making key, return code: %d\n",wc_ecc_make_key(&rng, KEY_SIZE_, &key));
// // //         return EXIT_FAILURE;
// // //     }
// // //     printf("\nKey made successfully, return code: %d \n",wc_ecc_make_key(&rng, KEY_SIZE_, &key));
// // //     wc_FreeRng(&rng);
// // //     return EXIT_SUCCESS;
// // // }

// // // // Function to sign data
// // // int sign(const uint8_t *data, size_t len, uint8_t *signature) {
// // //     WC_RNG rng;
// // //     wc_InitRng(&rng);
// // //     word32 sigSz = SIGNATURE_SIZE;
// // //     printf("\n**********From Sign Fucntion**********\n");
// // //     printf("\nThe value of sigSz variable before signing is: %u\n", sigSz);
// // //     printf("\nData to be signed: ");
// // //     for (size_t i = 0; i < len; i++) {
// // //         printf("%c", data[i]);
// // //     }
// // //     int key_ok=wc_ecc_check_key(&key);
// // //     if(key_ok == MP_OKAY){
// // //         printf("\nKey is not Fucked up\n");
// // //     }
// // //     else{
// // //         printf("\nKey is Fucked up\n");
// // //     }
// // //     int ret = wc_ecc_sign_hash(data, len, signature, &sigSz, &rng, &key);
// // //     printf("\nThe value of sigSz variable after signing is: %u\n", sigSz);
// // //     updated_sigSz = sigSz;
// // //     printf("\nReturn Value of Sign function(0 is Good): %d\n", ret);
// // //     wc_FreeRng(&rng);
// // //     if (ret != 0) {
// // //         printf("\nError signing data\n");
// // //         return -1;
// // //     }
// // //     // Print the signature
// // //     printf("\nSignature: ");
// // //     size_t i = 0;
// // //     for (i = 0; i < sigSz; i++)
// // //     {
// // //         printf("%02x", signature[i]);
// // //     }printf("\n");
// // //     printf("\nSignature Size after Signing: %zu", i);
// // //     printf("\n");
// // //     return 0;
// // // }

// // // // Function to verify signature
// // // int verify_signature(const uint8_t *data, size_t len, uint8_t *signature) {
// // //     int result;
// // //     int key_ok=wc_ecc_check_key(&key);
// // //     word32 sigSz = updated_sigSz;
// // //     printf("\n**********From Verify Fucntion**********\n");
// // //     printf("\nThe value of sigSz variable before verifying is: %u\n", sigSz);
// // //     printf("\nData to be verified: ");
// // //     for (size_t i = 0; i < len; i++) {
// // //         printf("%c", data[i]);
// // //     }
// // //     if (key_ok == MP_OKAY)
// // //     {
// // //         printf("\nKey is not Fucked up\n");
// // //     }
// // //     else{
// // //         printf("\nKey is Fucked up\n");
// // //     }
// // //     // Print the signature
// // //     printf("\nSignature: ");
// // //     size_t i = 0;
// // //     for (i = 0; i < sigSz; i++)
// // //     {
// // //         printf("%02x", signature[i]);
// // //     }
// // //     printf("\n");
// // //     printf("\nThe value of sigSz variable before verifying is: %u\n", sigSz);
// // //     printf("\n");
// // //     int ret = wc_ecc_verify_hash(signature, sigSz, data, len, &result, &key);
// // //     printf("\nThe value of sigSz variable after verifying is: %u\n", sigSz);
// // //     if (ret != 0) {
// // //         printf("\nVerify function Return Value: %d\n", ret);
// // //         printf("\nError performing the signature verification\n");
// // //         return -1;
// // //     }
// // //     printf("\n%s\n", data);
// // //     printf("\n%d is the return value of the verify_hash function\n", ret);
// // //     return result; // 1 for success, 0 for failure
// // // }

// // // int main() {
// // //     const uint8_t data[] = "Hello Darkness, My old friend!";
// // //     printf("\noriginal Data length: %d\n",(int)sizeof(data));
// // // // uint8_t signature[SIGNATURE_SIZE];

// // // // Initialize ECC keys
// // // // initialize_key();

// // // //printing the keys:
// // // // print_ecc_key("\n\nkey",&key);
// // // // printf("\n\n");
// // // // printf("\n");
// // // // // Sign the data

// // // // if (sign(data, sizeof(data), signature) != 0) {
// // // //     printf("\nFailed to sign data\n");
// // // //     exit(EXIT_FAILURE);
// // // // }

// // // // // Verify the signature
// // // // int verification_result = verify_signature(data, sizeof(data), signature);
// // // // printf("\n\n");
// // // // printf("\nValid Signature result (1 is Authentic): %d\n", verification_result);
// // // // if (verification_result == 1) {
// // // //     printf("\nSignature verification successful\n");
// // // // } else if (verification_result == 0) {
// // // //     printf("\nSignature verification failed\n");
// // // // }

// // // // Print the value of data[] as a string
// // // printf("\nValue of data[] as a string: %s\n", data);
// // //     // uint8_t signature[SIGNATURE_SIZE];

// // //     // Initialize ECC keys
// // //     int status=initialize_key();
// // //     if(status!=EXIT_SUCCESS){
// // //         printf("\nError in initializing the key: %d\n",status);
// // //         return EXIT_FAILURE;
// // //     }
// // //     printf("%02X,%zu", key.pubkey,sizeof(key.pubkey));
// // //     ecc_point* pub_key = &key.pubkey;
// // //     printf("\nAt infinity check: %d", wc_ecc_point_is_at_infinity(pub_key));

// // //     //printing the keys:
// // //     print_ecc_key("\n\nGenerated",&key);
// // //     // printf("\n\n");
// // //     // printf("\n");
// // //     // // Sign the data
    
// // //     // if (sign(data, sizeof(data), signature) != 0) {
// // //     //     printf("\nFailed to sign data\n");
// // //     //     exit(EXIT_FAILURE);
// // //     // }

// // //     // // Verify the signature
// // //     // int verification_result = verify_signature(data, sizeof(data), signature);
// // //     // printf("\n\n");
// // //     // printf("\nValid Signature result (1 is Authentic): %d\n", verification_result);
// // //     // if (verification_result == 1) {
// // //     //     printf("\nSignature verification successful\n");
// // //     // } else if (verification_result == 0) {
// // //     //     printf("\nSignature verification failed\n");
// // //     // } else {
// // //     //     printf("\nError verifying signature\n");
// // //     //     exit(EXIT_FAILURE);
// // //     // }
    
// // //     printf("Entered Secure Send from application processor\n");

// // //     // Example data array
// // //     // uint8_t data[] = "Example data to sign";

// // //     uint8_t signat[SIGNATURE_SIZE];
// // //     if (sign(data, sizeof(data), signat) != 0) {
// // //         printf("\nFailed in the sign function");
// // //         return -1;
// // //     }

// // //     uint8_t signed_packet[sizeof(data) + SIGNATURE_SIZE];
// // //     memcpy(signed_packet, data, sizeof(data));
// // //     memcpy(signed_packet + sizeof(data), signat, SIGNATURE_SIZE);

// // //     // Print the signed packet as plain text
// // //     printf("Signed packet:\n");
// // //     for (size_t i = 0; i < sizeof(signed_packet); i++) {
// // //         printf("%c", signed_packet[i]);
// // //     }
// // //     printf("\n");

// // //     printf("This would be secure receive\n");

// // //     size_t r_len = sizeof(signed_packet);
// // //     uint8_t r_sign[SIGNATURE_SIZE];
// // //     memcpy(r_sign, signed_packet + r_len - SIGNATURE_SIZE, SIGNATURE_SIZE);

// // //     if (verify_signature(signed_packet, sizeof(signed_packet), r_sign) != 0) {
// // //         printf("\nVERIFICATION SUCESSFUL VALID DATA\n");
// // //     }

// // //     // Calculate the length of the received data
// // //     int len = r_len - SIGNATURE_SIZE;
// // //     printf("Received data length: %d\n", len);

// // //     // int data;
// // //     // data=MXC_TRNG_RandomInt();
// // //     // printf("%d",data);
// // //     return 0;
// // // }

// // // // EXTRA_CFLAGS='-UHAVE___UINT128_T' ./configure \
// // // // --host=arm-non-eabi \
// // // // CC=arm-none-eabi-gcc LD=arm-none-eabi-ld \
// // // // AR=arm-none-eabi-ar RANLIB=arm-none-eabi-ranlib\
// // // // STRIP=arm-none-eabi-strip \
// // // // RANLIB=arm-none-eabi-ranlib \
// // // // CFLAGS="--specs=nosys.specs -mcpu=cortex-m4 -mthumb \
// // // // -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os \
// // // // -DNO_WOLFSSL_DIR -DHAVE_PK_CALLBACKS -DWOLFSSL_USER_IO -D_POSIX_C_SOURCE=199309L -DNO_WRITEV" \
// // // // LDFLAGS="-Lrt"\
// // // // --disable-filesystem --enable-fastmath --enable-harden --enable-ecc --enable-keygen --enable-debug\
// // // // --disable-shared && make

// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include <stdint.h>

// // #define MESSAGE_SIZE 100
// // #define SIGNATURE_SIZE 64

// // typedef struct {
// //     unsigned char private_key[32];
// //     unsigned char public_key[64];
// // } ECC_Keypair;

// // uint8_t generate_random() {
// //     return rand() % 256;
// // }

// // void generate_hash(const unsigned char *message, unsigned int *hash) {
// //     // Simulating hash generation by summing ASCII values of characters in the message
// //     *hash = 0;
// //     for (int i = 0; i < MESSAGE_SIZE && message[i] != '\0'; i++) {
// //         *hash += (unsigned int)message[i];
// //     }
// // }

// // void sign(const unsigned char *message, const unsigned char *private_key, unsigned char *signature) {
// //     unsigned int hash;
// //     generate_hash(message, &hash);
// //     memcpy(signature, &hash, sizeof(unsigned int));
// // }

// // int verify(const unsigned char *message, const unsigned char *signature, const unsigned char *public_key) {
// //     unsigned int hash;
// //     generate_hash(message, &hash);

// //     unsigned int stored_hash;
// //     memcpy(&stored_hash, signature, sizeof(unsigned int));

// //     return (hash == stored_hash);
// // }

// // int main() {
// //     ECC_Keypair keypair;
// //     for (int i = 0; i < sizeof(keypair.private_key); i++) {
// //         keypair.private_key[i] = generate_random();
// //     }
// //     for (int i = 0; i < sizeof(keypair.public_key); i++) {
// //         keypair.public_key[i] = generate_random();
// //     }

// //     const unsigned char original_message[MESSAGE_SIZE] = "Hello, world!";
// //     unsigned char retrieved_message[MESSAGE_SIZE]; // Buffer to store retrieved message

// //     unsigned char signature[SIGNATURE_SIZE];
// //     sign(original_message, keypair.private_key, signature);

// //     int is_valid = verify(original_message, signature, keypair.public_key);

// //     if (is_valid) {
// //         printf("Message is authentic and integrity is intact.\n");
// //         // If the message is valid, retrieve the original message from the buffer
// //         memcpy(retrieved_message, original_message, MESSAGE_SIZE);
// //         printf("Retrieved message: %s\n", retrieved_message);
// //     } else {
// //         printf("Message authentication failed or integrity compromised.\n");
// //     }

// //     return 0;
// // }

// #include <stdio.h>
// #include <string.h>
// #include <wolfssl/options.h>
// #include <wolfssl/wolfcrypt/hmac.h>
// #include "mxc_device.h"
// #include "nvic_table.h"
// #include "trng.h"


// // Define the size of the random number generated
// #define RANDOM_SIZE 8

// // Global variable for random number
// unsigned char var_rnd_no[RANDOM_SIZE];

// volatile int wait;
// volatile int callback_result;
// // Custom TRNG function prototypes
// // Custom TRNG test function
// void Test_TRNG(int asynchronous) {

//         print_info(asynchronous ? "\nTest TRNG Async\n" : "\nTest TRNG Sync\n");
//         int num_bytes = 8;
//         memset(var_rnd_no, 0, sizeof(var_rnd_no));
//         MXC_TRNG_Init();
//         if (asynchronous) {
//             wait = 1;
//             NVIC_EnableIRQ(TRNG_IRQn);
//             MXC_TRNG_RandomAsync(var_rnd_no, num_bytes, &Test_Callback);
//             while (wait) {}
//         }else {
//             MXC_TRNG_Random(var_rnd_no, num_bytes);
//         }
//         // print((char *)var_rnd_no);
//         MXC_TRNG_Shutdown();
//         print_info("TRNG Test function Complete\n");
//     }

// // Function to generate an HMAC for a message
// unsigned char* generate_hmac(const unsigned char* message, size_t message_len, const unsigned char* key, size_t key_len) {
//     unsigned char* hmac_result = (unsigned char*)malloc(SHA256_DIGEST_SIZE);
//     wc_HmacSha256(message, message_len, key, key_len, hmac_result);
//     return hmac_result;
// }

// // Function to verify the HMAC of a message
// int verify_hmac(const unsigned char* message, size_t message_len, const unsigned char* hmac, size_t hmac_len, const unsigned char* key, size_t key_len) {
//     unsigned char computed_hmac[SHA256_DIGEST_SIZE];
//     wc_HmacSha256(message, message_len, key, key_len, computed_hmac);
//     return memcmp(computed_hmac, hmac, hmac_len) == 0;
// }

// int main() {
//     // Example usage
//     unsigned char shared_key[] = "shared_secret_key";
    
//     // Test random number generation
//     Test_TRNG();
//     printf("Random Number: ");
//     for (int i = 0; i < RANDOM_SIZE; i++) {
//         printf("%02x", var_rnd_no[i]);
//     }
//     printf("\n");
    
//     // Simulate a message
//     const unsigned char* message = "Hello, World!";
//     size_t message_len = strlen(message);
    
//     // Generate the HMAC for the message
//     unsigned char* hmac = generate_hmac(message, message_len, shared_key, sizeof(shared_key) - 1);
    
//     // Verify the HMAC
//     int verified = verify_hmac(message, message_len, hmac, SHA256_DIGEST_SIZE, shared_key, sizeof(shared_key) - 1);
    
//     if (verified) {
//         printf("HMAC verified successfully!\n");
//     } else {
//         printf("HMAC verification failed!\n");
//     }
    
//     // Free allocated memory
//     free(hmac);
    
//     return 0;
// }

#include <stdio.h>
#include <string.h>

// Shared secret key between Application Processor and Component
unsigned char shared_key[] = "asdfdf90803q4p5'l;";

// Function to sign a message
void sign_message(const unsigned char* message, size_t message_len, unsigned char* signature) {
    // Custom signing algorithm (e.g., simple concatenation with the shared key)
    memcpy(signature, message, message_len);
    memcpy(signature + message_len, shared_key, sizeof(shared_key));
    // In practice, use a more secure signing algorithm such as HMAC or digital signatures (e.g., RSA)
}

// Function to verify the signature of a message
int verify_signature(const unsigned char* message, size_t message_len, const unsigned char* signature) {
    unsigned char expected_signature[message_len + sizeof(shared_key)];
    // Generate the expected signature using the same custom algorithm
    sign_message(message, message_len, expected_signature);
    // Compare the received signature with the expected signature
    return memcmp(signature, expected_signature, sizeof(expected_signature)) == 0;
}

int main() {
    // Example usage
    const unsigned char* message = "Hello, Component!";
    size_t message_len = strlen(message);
    
    // Sign the message
    unsigned char signature[message_len + sizeof(shared_key)];
    sign_message(message, message_len, signature);
    
    // Simulate transmission of the message and signature to the Component
    
    // Simulate the Component receiving the message and signature
    // Verify the signature
    int verified = verify_signature(message, message_len, signature);
    
    if (verified) {
        printf("Signature verified successfully!\n");
        // Proceed with processing the message
    } else {
        printf("Signature verification failed!\n");
        // Discard or handle the message accordingly
    }
    
    return 0;
}
