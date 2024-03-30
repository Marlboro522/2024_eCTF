#include"../../application_processor/inc/simple_crypto.h"
#define SUCCESS_RETURN 0
#define ERROR_RETURN -1
// variable to store signature
#include <stdint.h>
#include <string.h>
uint8_t signature[SIGNATURE_SIZE];
word32 updated_sigSz;
ecc_key comm_key;
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
