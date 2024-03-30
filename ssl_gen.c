#include <wolfssl/options.h>
// // #include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/ecc.h>
#include <stdio.h>
// #include<wolfssl/wolfcrypt/random.h>
// #include <stdlib.h>

#define KEY_SIZE_ 16 // 32 bytes for 128-bit keys
#define SIGNATURE_SIZE 64 // 64 bytes for 512-bit ECDSA signatures
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
word32 updated_sigSz;
// Global variables for storing keys
ecc_key key;

void print_ecc_key(const char *label, ecc_key *key1) {
    byte buffer[1024]; // Buffer to hold exported key
    word32 bufferSz = sizeof(buffer);
    if (wc_ecc_export_x963(key1, buffer, &bufferSz) != 0) {
        printf("Error exporting %s key\n", label);
        return;
    }
    printf("\n%s key: ", label);
    for (int i = 0; i < bufferSz; i++) {
        printf("%02X", buffer[i]);
    }
    printf("\n\n");
}
void initialize_key() {
    wc_ecc_init(&key);
    WC_RNG rng;
    wc_InitRng(&rng);
    if (wc_ecc_make_key(&rng, KEY_SIZE_, &key) != 0) {
        printf("\nError making key, return code: %d\n",wc_ecc_make_key(&rng, KEY_SIZE_, &key));
        exit(EXIT_FAILURE);
    }else{
        printf("\nKey made successfully, return code: %d \n",wc_ecc_make_key(&rng, KEY_SIZE_, &key));
    }
    wc_FreeRng(&rng);
}

// Function to sign data
int sign(const uint8_t *data, size_t len, uint8_t *signature) {
    WC_RNG rng;
    wc_InitRng(&rng);
    word32 sigSz = SIGNATURE_SIZE;
    printf("\n**********From Sign Fucntion**********\n");
    printf("\nThe value of sigSz variable before signing is: %u\n", sigSz);
    printf("\nData to be signed: ");
    for (size_t i = 0; i < len; i++) {
        printf("%c", data[i]);
    }
    int key_ok=wc_ecc_check_key(&key);
    if(key_ok == MP_OKAY){
        printf("\nKey is not Fucked up\n");
    }
    else{
        printf("\nKey is Fucked up\n");
    }
    int ret = wc_ecc_sign_hash(data, len, signature, &sigSz, &rng, &key);
    printf("\nThe value of sigSz variable after signing is: %u\n", sigSz);
    updated_sigSz = sigSz;
    printf("\nReturn Value of Sign function(0 is Good): %d\n", ret);
    wc_FreeRng(&rng);
    if (ret != 0) {
        printf("\nError signing data\n");
        return -1;
    }
    // Print the signature
    printf("\nSignature: ");
    size_t i = 0;
    for (i = 0; i < sigSz; i++)
    {
        printf("%02x", signature[i]);
    }printf("\n");
    printf("\nSignature Size after Signing: %zu", i);
    printf("\n");
    return 0;
}

// Function to verify signature
int verify_signature(const uint8_t *data, size_t len, uint8_t *signature) {
    int result;
    int key_ok=wc_ecc_check_key(&key);
    word32 sigSz = updated_sigSz;
    printf("\n**********From Verify Fucntion**********\n");
    printf("\nThe value of sigSz variable before verifying is: %u\n", sigSz);
    printf("\nData to be verified: ");
    for (size_t i = 0; i < len; i++) {
        printf("%c", data[i]);
    }
    if (key_ok == MP_OKAY)
    {
        printf("\nKey is not Fucked up\n");
    }
    else{
        printf("\nKey is Fucked up\n");
    }
    // Print the signature
    printf("\nSignature: ");
    size_t i = 0;
    for (i = 0; i < sigSz; i++)
    {
        printf("%02x", signature[i]);
    }
    printf("\n");
    printf("\nThe value of sigSz variable before verifying is: %u\n", sigSz);
    printf("\n");
    int ret = wc_ecc_verify_hash(signature, sigSz, data, len, &result, &key);
    printf("\nThe value of sigSz variable after verifying is: %u\n", sigSz);
    if (ret != 0) {
        printf("\nVerify function Return Value: %d\n", ret);
        printf("\nError performing the signature verification\n");
        return -1;
    }
    printf("\n%d is the return value of the verify_hash function\n",ret);
    return result; // 1 for success, 0 for failure
}

int main() {
    const uint8_t data[] = "Hello Darkness, My old friend!";
    printf("\noriginal Data length: %d\n",(int)sizeof(data));
// uint8_t signature[SIGNATURE_SIZE];

// Initialize ECC keys
// initialize_key();

//printing the keys:
// print_ecc_key("\n\nkey",&key);
// printf("\n\n");
// printf("\n");
// // Sign the data

// if (sign(data, sizeof(data), signature) != 0) {
//     printf("\nFailed to sign data\n");
//     exit(EXIT_FAILURE);
// }

// // Verify the signature
// int verification_result = verify_signature(data, sizeof(data), signature);
// printf("\n\n");
// printf("\nValid Signature result (1 is Authentic): %d\n", verification_result);
// if (verification_result == 1) {
//     printf("\nSignature verification successful\n");
// } else if (verification_result == 0) {
//     printf("\nSignature verification failed\n");
// }

// Print the value of data[] as a string
printf("\nValue of data[] as a string: %s\n", data);
    // uint8_t signature[SIGNATURE_SIZE];

    // Initialize ECC keys
    initialize_key();
    printf("%02X,%zu", key.pubkey,sizeof(key.pubkey));
    ecc_point* pub_key = &key.pubkey;
    printf("\nAt infinity check: %d", wc_ecc_point_is_at_infinity(pub_key));

    //printing the keys:
    print_ecc_key("\n\nGenerated",&key);
    // printf("\n\n");
    // printf("\n");
    // // Sign the data
    
    // if (sign(data, sizeof(data), signature) != 0) {
    //     printf("\nFailed to sign data\n");
    //     exit(EXIT_FAILURE);
    // }

    // // Verify the signature
    // int verification_result = verify_signature(data, sizeof(data), signature);
    // printf("\n\n");
    // printf("\nValid Signature result (1 is Authentic): %d\n", verification_result);
    // if (verification_result == 1) {
    //     printf("\nSignature verification successful\n");
    // } else if (verification_result == 0) {
    //     printf("\nSignature verification failed\n");
    // } else {
    //     printf("\nError verifying signature\n");
    //     exit(EXIT_FAILURE);
    // }
    
    printf("Entered Secure Send from application processor\n");

    // Example data array
    // uint8_t data[] = "Example data to sign";

    uint8_t signat[SIGNATURE_SIZE];
    if (sign(data, sizeof(data), signat) != 0) {
        printf("\nFailed in the sign function");
        return -1;
    }

    uint8_t signed_packet[sizeof(data) + SIGNATURE_SIZE];
    memcpy(signed_packet, data, sizeof(data));
    memcpy(signed_packet + sizeof(data), signat, SIGNATURE_SIZE);

    // Print the signed packet as plain text
    printf("Signed packet:\n");
    for (size_t i = 0; i < sizeof(signed_packet); i++) {
        printf("%c", signed_packet[i]);
    }
    printf("\n");

    printf("\nThis would be secure receive\n");

    size_t r_len = sizeof(signed_packet);
    uint8_t r_sign[SIGNATURE_SIZE];
    memcpy(r_sign, signed_packet + r_len - SIGNATURE_SIZE, SIGNATURE_SIZE);

    if (verify_signature(signed_packet, sizeof(signed_packet), r_sign) != 0) {
        printf("\nVERIFICATION SUCESSFUL VALID DATA\n");
    }

    // Calculate the length of the received data
    int len = r_len - SIGNATURE_SIZE;
    printf("Received data length: %d\n", len);

    return 0;
}
