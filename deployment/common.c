#include "common.h"
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include<wolfssl/wolfcrypt/random.h>
#include<wolfssl/options.h>
// #include<wolfssl/ssl.h>
#include<wolfssl/wolfcrypt/ecc.h>
#include <wolfssl/wolfcrypt/asn.h>
#include <wolfssl/wolfcrypt/types.h>
#define KEY_SIZE_ 16
void initialize_key() {
    wc_ecc_init(&key);
    WC_RNG rng;
    wc_InitRng(&rng);
    if (wc_ecc_make_key(&rng, KEY_SIZE_, &key) != 0) {
    }
    wc_FreeRng(&rng);
}