#include"../../application_processor/inc/simple_crypto.h"

#define SUCCESS_RETURN 0
#define ERROR_RETURN -1
#define MAX_LEN 64
// variable to store signature
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "global_secrets.h"

int sign_message(uint8_t* message, size_t message_len, unsigned char* signature){
    if (message == NULL || signature == NULL || shared_secret[0] == '\0') {
        return ERROR_RETURN;
    }
    size_t max_signature_len = message_len + sizeof(shared_secret);
    if (max_signature_len > MAX_LEN) {
        return ERROR_RETURN; // Error code for exceeding maximum length
    }
    memcpy(signature, message, message_len);
    memcpy(signature + message_len, shared_secret, sizeof(shared_secret));
    return SUCCESS_RETURN;
}
    // Function to verify the signature of a message
int verify_signature(uint8_t* message, size_t message_len, unsigned char* signature) {

    if(message==NULL || signature==NULL || shared_secret[0]=='\0'){
        return ERROR_RETURN;
    }
    unsigned char expected_signature[SIGNATURE_SIZE];
    if(sign_message(message, message_len, expected_signature)!=0){
        return ERROR_RETURN;
    }
    int m = memcmp(signature, expected_signature, sizeof(expected_signature));
    if (m!= 0) {
        return m;
    }
    return SUCCESS_RETURN;
}