/**
 * @file component.c
 * @author Jacob Doll 
 * @brief eCTF Component Example Design Implementation
 * @date 2024
 *
 * This source file is part of an example system for MITRE's 2024 Embedded System CTF (eCTF).
 * This code is being provided only for educational purposes for the 2024 MITRE eCTF competition,
 * and may not meet MITRE standards for quality. Use this code at your own risk!
 *
 * @copyright Copyright (c) 2024 The MITRE Corporation
 */

#include "board.h"
#include "i2c.h"
#include "led.h"
#include "mxc_delay.h"
#include "mxc_errors.h"
#include "nvic_table.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "simple_i2c_peripheral.h"
#include "board_link.h"

// Includes from containerized build
#include "ectf_params.h"
#include "global_secrets.h"

#ifdef POST_BOOT
#include "led.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#endif

#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/asn.h>
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/rsa.h>

/********************************* CONSTANTS **********************************/

// Passed in through ectf-params.h
// Example of format of ectf-params.h shown here
/*
#define COMPONENT_ID 0x11111124
#define COMPONENT_BOOT_MSG "Component boot"
#define ATTESTATION_LOC "McLean"
#define ATTESTATION_DATE "08/08/08"
#define ATTESTATION_CUSTOMER "Fritz"
*/

/******************************** TYPE DEFINITIONS ********************************/
// Commands received by Component using 32 bit integer
typedef enum {
    COMPONENT_CMD_NONE,
    COMPONENT_CMD_SCAN,
    COMPONENT_CMD_VALIDATE,
    COMPONENT_CMD_BOOT,
    COMPONENT_CMD_ATTEST
} component_cmd_t;

/******************************** TYPE DEFINITIONS ********************************/
// Data structure for receiving messages from the AP
typedef struct {
    uint8_t opcode;
    uint8_t params[MAX_I2C_MESSAGE_LEN-1];
} command_message;

typedef struct {
    uint32_t component_id;
} validate_message;

typedef struct {
    uint32_t component_id;
} scan_message;

/********************************* FUNCTION DECLARATIONS **********************************/
// Core function definitions
void component_process_cmd(void);
void process_boot(void);
void process_scan(void);
void process_validate(void);
void process_attest(void);

/********************************* GLOBAL VARIABLES **********************************/
// Global varaibles
uint8_t receive_buffer[MAX_I2C_MESSAGE_LEN];
uint8_t transmit_buffer[MAX_I2C_MESSAGE_LEN];

/******************************* POST BOOT FUNCTIONALITY *********************************/
#define KEY_SIZE_ 512
#define SIGNATURE_SIZE 64
/**
 * @brief Create Cert
 * 
 * Create a cert used for verification of identity

*/
/*
void* create_cert(){

    Initialize cert
    Cert* newCert;
    wc_InitCert(newCert);

    Initialize cert info
    strncpy(myCert.subject.country, "US", CTC_NAME_SIZE);
    strncpy(myCert.subject.state, "CO", CTC_NAME_SIZE);
    strncpy(myCert.subject.locality, "Colorado Springs", CTC_NAME_SIZE);
    strncpy(myCert.subject.org, "RGB", CTC_NAME_SIZE); //Change
    strncpy(myCert.subject.unit, "CTF", CTC_NAME_SIZE); //change
    strncpy(myCert.subject.commonName, "www.uccs.edu", CTC_NAME_SIZE); //change
    strncpy(myCert.subject.email, "kzytka@uccs.edu", CTC_NAME_SIZE); //change

    generate key and rng
    RsaKey key;
    RNG    rng;
    int    ret;

    wc_InitRng(&rng);
    wc_InitRsaKey(&key, 0);

    ret = wc_MakeRsaKey(&key, 1024, 65537, &rng);
    if (ret != 0)
        fprintf(stderr, "not able to make key.\n");

    generate self signed cert
    byte derCert[4096];

    int certSz = wc_MakeSelfCert(&myCert, derCert, sizeof(derCert), &key, &rng);
    if (certSz < 0){
        fprintf(stderr, "cannot make cert.\n");
        exit(EXIT_FAILURE);
    }//if
    return derCert;

}
*/
int sign(uint8_t *data, uint8_t len,ecc_key* private_key,ecc_key* public_key, uint8_t *sign) { 
    int ret;
    int pubKey;
    wc_ecc_init(private_key);
    ret =
        wc_ecc_sign_hash(data, len, sign, SIGNATURE_SIZE, private_key);
    if(ret!=0){
        print_error("Failure...s");
    }
    pubKey = wc_ecc_export_x963(public_key, sign, KEY_SIZE_);
    return pubKey;
}
int sign_veriffy(uint8_t* data, uint8_t len,uint8_t* sign){
    int ret;
    ret = wc_ecc_verify_hash(sign, SIGNATURE_SIZE, data, len, private_key);
    if(ret!=0){
        print_error("Failure...v");
    }return ret;
}
/**
 * @brief Secure Send 
 * 
 * @param buffer: uint8_t*, pointer to data to be send
 * @param len: uint8_t, size of data to be sent 
 * 
 * Securely send data over I2C. This function is utilized in POST_BOOT functionality.
 * This function must be implemented by your team to align with the security requirements.
*/
void secure_send(uint8_t* buffer, uint8_t len) {

    uint8_t signat[SIGNATURE_SIZE];
    if(sign(buffer,len,private_key,public_key,signat)!=0){
        return ERROR_RETURN;
    } uint8_t signed_packet[len+SIGNATURE_SIZE];
    memcpy(signed_packet,buffer,len);
    memcpy(signed_packet + len, signat, SIGNATURE_SIZE);  

    send_packet_and_ack(len+SIGNATURE_SIZE, buffer); 
}

/**
 * @brief Secure Receive
 * 
 * @param buffer: uint8_t*, pointer to buffer to receive data to
 * 
 * @return int: number of bytes received, negative if error
 * 
 * Securely receive data over I2C. This function is utilized in POST_BOOT functionality.
 * This function must be implemented by your team to align with the security requirements.
*/
int secure_receive(uint8_t* buffer) {
    int r_len=wait_and_receive_packet(buffer);
    if(r_len<SUCCESS_RETURN){
        return ERROR_RETURN;
    }uint8_t r_sign[SIGNATURE_SIZE];
    memcpy(r_sign, buffer+r_len - SIGNATURE_SIZE, SIGNATURE_SIZE);
    if(sign_veriffy(buffer,r_len - SIGNATURE_SIZE,r_sign)!=0){
        return ERROR_RETURN;
    }
    int len = r_len - SIGNATURE_SIZE;
    return len;
}

/******************************* FUNCTION DEFINITIONS *********************************/

// Example boot sequence
// Your design does not need to change this
void boot() {

    // POST BOOT FUNCTIONALITY
    // DO NOT REMOVE IN YOUR DESIGN
    #ifdef POST_BOOT
        POST_BOOT
    #else
    // Anything after this macro can be changed by your design
    // but will not be run on provisioned systems
    LED_Off(LED1);
    LED_Off(LED2);
    LED_Off(LED3);
    // LED loop to show that boot occurred
    while (1) {
        LED_On(LED1);
        MXC_Delay(500000);
        LED_On(LED2);
        MXC_Delay(500000);
        LED_On(LED3);
        MXC_Delay(500000);
        LED_Off(LED1);
        MXC_Delay(500000);
        LED_Off(LED2);
        MXC_Delay(500000);
        LED_Off(LED3);
        MXC_Delay(500000);
    }
    #endif
}

// Handle a transaction from the AP
void component_process_cmd() {
    command_message* command = (command_message*) receive_buffer;

    // Output to application processor dependent on command received
    switch (command->opcode) {
    case COMPONENT_CMD_BOOT:
        process_boot();
        break;
    case COMPONENT_CMD_SCAN:
        process_scan();
        break;
    case COMPONENT_CMD_VALIDATE:
        process_validate();
        break;
    case COMPONENT_CMD_ATTEST:
        process_attest();
        break;
    default:
        printf("Error: Unrecognized command received %d\n", command->opcode);
        break;
    }
}

void process_boot() {
    // The AP requested a boot. Set `component_boot` for the main loop and
    // respond with the boot message
    uint8_t len = strlen(COMPONENT_BOOT_MSG) + 1;
    memcpy((void*)transmit_buffer, COMPONENT_BOOT_MSG, len);
    send_packet_and_ack(len, transmit_buffer);
    // Call the boot function
    boot();
}

void process_scan() {
    // The AP requested a scan. Respond with the Component ID
    scan_message* packet = (scan_message*) transmit_buffer;
    packet->component_id = COMPONENT_ID;
    send_packet_and_ack(sizeof(scan_message), transmit_buffer);
}

void process_validate() {
    // The AP requested a validation. Respond with the Component ID
    validate_message* packet = (validate_message*) transmit_buffer;
    packet->component_id = COMPONENT_ID;
    send_packet_and_ack(sizeof(validate_message), transmit_buffer);
}

void process_attest() {
    // The AP requested attestation. Respond with the attestation data
    uint8_t len = sprintf((char*)transmit_buffer, "LOC>%s\nDATE>%s\nCUST>%s\n",
                ATTESTATION_LOC, ATTESTATION_DATE, ATTESTATION_CUSTOMER) + 1;
    send_packet_and_ack(len, transmit_buffer);
}

/*********************************** MAIN *************************************/

int main(void) {
    printf("Component Started\n");
    
    // Enable Global Interrupts
    __enable_irq();
    
    // Initialize Component
    i2c_addr_t addr = component_id_to_i2c_addr(COMPONENT_ID);
    board_link_init(addr);
    
    LED_On(LED2);

    while (1) {
        wait_and_receive_packet(receive_buffer);

        component_process_cmd();
    }
}
