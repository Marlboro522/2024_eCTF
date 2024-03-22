#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/ecc.h>
#include <stdio.h>
#include <stdlib.h>

#define KEY_SIZE_ 16
#define SIGNATURE_SIZE 64

ecc_key pub;
ecc_key private;

void initialize_keys(){
    wc_ecc_init(&pub);
    wc_ecc_init(&private);
    WC_RNG rng;
    wc_InitRng(&rng);
    if (wc_ecc_make_key(&rng, 16, &private) != 0)
    {
        printf("\nError making sender key");
    }
}

int sign(uint8_t *data, uint8_t len, ecc_key* private_key, ecc_key* public_key, uint8_t *sign) { 
    int ret;
    WC_RNG rng; // Declare random number generator object
    wc_InitRng(&rng); // Initialize RNG
    wc_ecc_init(private_key);
    ret = wc_ecc_sign_hash(data, (word32)len, (byte *) sign, (word32 *) SIGNATURE_SIZE, &rng, private_key); // Pass RNG as last argument
    if (ret != 0) {
        printf("\nFailure signing");
    }
    wc_FreeRng(&rng); // Free RNG after use
    return wc_ecc_export_x963(public_key, sign, (word32 *) KEY_SIZE_);
}

int sign_veriffy(uint8_t* data, uint8_t len, uint8_t* sign) {
    int ret;
    int result;
    ret = wc_ecc_verify_hash((byte *)sign, (word32)SIGNATURE_SIZE, (byte *)data, (word32) len, &result, &pub);
    if (ret != 0) {
        printf("\nFailure verifying");
    }
    return result;
}

// Testing for PKI implemented in application_processor.c
int main(){
    uint8_t data[16] = "Hello, World!";
    uint8_t sign1[SIGNATURE_SIZE];
    initialize_keys();
    sign(data, 16, &private, &pub, sign1);
    printf("\nSignature: ");
    for (int i = 0; i < SIGNATURE_SIZE; i++) {
        printf("%02x", sign1[i]);
    }
    printf("\n");
    printf("\nVerification: %d\n", sign_veriffy(data, 16, sign1));
    return 0;
}
