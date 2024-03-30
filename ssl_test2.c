#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/ecc.h>
#include <stdio.h>
#include <stdlib.h>

#define KEY_SIZE_ 16 // 32 bytes for 128-bit keys
#define SIGNATURE_SIZE 64 // 64 bytes for 512-bit ECDSA signatures
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
word32 updated_sigSz;
// Global variables for storing keys
ecc_key pub;
ecc_key private_key;
ecc_key key;
// ecc_point pub;
// Function to initialize ECC keys
void print_ecc_key(const char *label, ecc_key *key) {
    byte buffer[1024]; // Buffer to hold exported key
    word32 bufferSz = sizeof(buffer);
    if (wc_ecc_export_x963(key, buffer, &bufferSz) != 0) {
        printf("Error exporting %s key\n", label);
        return;
    }
    printf("\n%s key: ", label);
    for (int i = 0; i < bufferSz; i++) {
        printf("%02X", buffer[i]);
    }
    printf("\n\n");
}
void initialize_keys() {
    wc_ecc_init(&key);
    wc_ecc_init(&pub);
    wc_ecc_init(&private_key);
    WC_RNG rng;
    wc_InitRng(&rng);
    byte priv[1024];
    word32 priv_sz=sizeof(priv);
    byte public[1024];
    word32 public_sz=sizeof(public);
    if (wc_ecc_make_key(&rng, KEY_SIZE_, &key) != 0)
    {
        printf("\nError making private key");
        exit(EXIT_FAILURE);
    }
    printf("\nPrivate Key after the initial make: \n");
    print_ecc_key("The Key struct\n\n", &key);
    if (wc_ecc_export_private_only(&key, priv, &priv_sz) != 0)
    {
        printf("\nError exporting Key to x963 format\n");
        exit(EXIT_FAILURE);
    }
    if (wc_ecc_export_x963(&key, public, &public_sz) != 0)
    {
        printf("\nError exporting Key to x963 format\n");
        exit(EXIT_FAILURE);
    }
    if (wc_ecc_import_x963(public,public_sz,&pub) != 0)
    {
        printf("\nError importing public key\n");
        exit(EXIT_FAILURE);
    }print_ecc_key("Public\n\n", &pub);
    if (wc_ecc_import_private_key(priv, priv_sz, public,public_sz,&private_key) != 0)
    {
        printf("\nError importing private key\n");
        exit(EXIT_FAILURE);
    } 
    print_ecc_key("\n\nPrivate\n\n", &private_key);
    wc_FreeRng(&rng);
}int main(){
    initialize_keys();
    return 0;
}