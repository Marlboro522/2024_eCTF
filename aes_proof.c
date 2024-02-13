#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/random.h>
#include <stdio.h>
#include <stdlib.h>

#define AES_KEY_SIZE 32 // 256-bit or something I guess
//Yep, this works right now, we need to find the secure_send and secure_recieve functions in the application processor.c and component.c 
//this is for encrypting the PIN, replcement token
//component id's should not be encrypted, since they are to be revealed in the attack phase. 
int main() {
    byte aes_key[AES_KEY_SIZE];
    WC_RNG rng;
    if (wc_InitRng(&rng) != 0) {
        fprintf(stderr, "Error with the function\n");
        return EXIT_FAILURE;
    }
    if (wc_RNG_GenerateBlock(&rng, aes_key, AES_KEY_SIZE) != 0) {
        fprintf(stderr, "Error with the key\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < AES_KEY_SIZE; ++i) {
        printf("%02x", aes_key[i]);
    }
    printf("\n");
    wc_FreeRng(&rng);

    return EXIT_SUCCESS;
}
