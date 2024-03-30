#include "ex_key.h"
#include <stdint.h>
#include <string.h>
uint8_t signature[SIGNATURE_SIZE];
word32 updated_sigSz;
ecc_key comm_key;
int initialize_key() {
    wc_ecc_init(&comm_key);
    WC_RNG rng;
    wc_InitRng(&rng);
    if (wc_ecc_make_key(&rng, KEY_SIZE_, &comm_key) != 0) {
        printf("failed in initialize_key\n");
        return SUCCESS_RETURN;
    }
    return ERROR_RETURN;
    wc_FreeRng(&rng);
}
int sign(const uint8_t *data, size_t len, uint8_t *signature) {
    WC_RNG rng;
    wc_InitRng(&rng);
    word32 sigSz = SIGNATURE_SIZE;
    int key_ok=wc_ecc_check_key(&comm_key);
    if(key_ok != MP_OKAY){
        printf("Key not OK");
        return ERROR_RETURN;
    }
    int ret = wc_ecc_sign_hash(data, len, signature, &sigSz, &rng, &comm_key);
    updated_sigSz = sigSz;
    wc_FreeRng(&rng);
    if (ret != 0) {
        printf("Signing Failed");
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