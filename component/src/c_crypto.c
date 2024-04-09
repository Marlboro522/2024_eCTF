#include"../../application_processor/inc/simple_crypto.h"

#define SUCCESS_RETURN 0
#define ERROR_RETURN -1
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
    memcpy(signature, message, message_len);
    memcpy(signature + message_len, shared_secret, sizeof(shared_secret));
    return SUCCESS_RETURN;
}
    // Function to verify the signature of a message
int verify_signature(uint8_t* message, size_t message_len, unsigned char* signature) {

    if(message==NULL || signature==NULL || shared_secret[0]=='\0'){
        return ERROR_RETURN;
    }
    unsigned char expected_signature[message_len + sizeof(shared_secret)];
    // Generate the expected signature using the same custom algorithm
    if(sign_message(message, message_len, expected_signature)!=0){
        return ERROR_RETURN;
    }
    if(memcmp(signature, expected_signature, sizeof(expected_signature))!=0){
        return ERROR_RETURN;
    }
    return SUCCESS_RETURN;
}