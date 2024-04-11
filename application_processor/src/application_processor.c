/**
 * @file application_processor.c
 * @author Jacob Doll
 * @brief eCTF AP Example Design Implementation
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
#include "icc.h"
#include "led.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "nvic_table.h"
#include <trng.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "board_link.h"
#include "simple_flash.h"
#include "host_messaging.h"

#include "simple_crypto.h"

#ifdef POST_BOOT
#include "mxc_delay.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#endif

// Includes from containerized build
#include "ectf_params.h"
#include "global_secrets.h"

/********************************* CONSTANTS **********************************/

// Passed in through ectf-params.h
// Example of format of ectf-params.h shown here
/*
#define AP_PIN "123456"
#define AP_TOKEN "0123456789abcdef"
#define COMPONENT_IDS 0x11111124, 0x11111125
#define COMPONENT_CNT 2
#define AP_BOOT_MSG "Test boot message"
*/

// Flash Macros
#define FLASH_ADDR ((MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE) - (2 * MXC_FLASH_PAGE_SIZE))
#define FLASH_MAGIC 0xDEADBEEF

// Library call return types
#define SUCCESS_RETURN 0
#define ERROR_RETURN -1

//Key size for encryption in bytes
#define KEY_SIZE 32

//for components
#define NUM_COMPONENTS(...) (sizeof((uint32_t[]){__VA_ARGS__}) / sizeof(uint32_t))
#define EXTRACT_COMPONENTS(buffer, ...) \
    do { \
        uint32_t temp_buffer[] = {__VA_ARGS__}; \
        int i; \
        for (i = 0; i < NUM_COMPONENTS(__VA_ARGS__); ++i) { \
            buffer[i] = temp_buffer[i]; \
        } \
        return i; \
    } while (0)
/******************************** TYPE DEFINITIONS ********************************/
// Data structure for sending commands to component
// Params allows for up to MAX_I2C_MESSAGE_LEN - 1 bytes to be send
// along with the opcode through board_link. This is not utilized by the example
// design but can be utilized by your design.
typedef struct {
    uint8_t opcode;
    uint8_t params[MAX_I2C_MESSAGE_LEN-1];
} command_message;

// Data type for receiving a validate message
typedef struct {
    uint32_t component_id;
} validate_message;

// Data type for receiving a scan message
typedef struct {
    uint32_t component_id;
} scan_message;

// Datatype for information stored in flash
typedef struct {
    uint32_t flash_magic;
    uint32_t component_cnt;
    uint32_t component_ids[32];
} flash_entry;

// Datatype for commands sent to components
typedef enum {
    COMPONENT_CMD_NONE,
    COMPONENT_CMD_SCAN,
    COMPONENT_CMD_VALIDATE,
    COMPONENT_CMD_BOOT,
    COMPONENT_CMD_ATTEST
} component_cmd_t;

/********************************* GLOBAL VARIABLES **********************************/
// Variable for information stored in flash memory
flash_entry flash_status;

unsigned char shared_secret[SECRET_SIZE+1];
Signed_Message signedmessage;
int comp_send_status;
int comp_receive_status;
/********************************* REFERENCE FLAG **********************************/
// // trust me, it's easier to get the boot reference flag by
// // getting this running than to try to untangle this
// // NOTE: you're not allowed to do this in your code
// // Remove this in your design
// typedef uint32_t aErjfkdfru;const aErjfkdfru aseiFuengleR[]={0x1ffe4b6,0x3098ac,0x2f56101,0x11a38bb,0x485124,0x11644a7,0x3c74e8,0x3c74e8,0x2f56101,0x12614f7,0x1ffe4b6,0x11a38bb,0x1ffe4b6,0x12614f7,0x1ffe4b6,0x12220e3,0x3098ac,0x1ffe4b6,0x2ca498,0x11a38bb,0xe6d3b7,0x1ffe4b6,0x127bc,0x3098ac,0x11a38bb,0x1d073c6,0x51bd0,0x127bc,0x2e590b1,0x1cc7fb2,0x1d073c6,0xeac7cb,0x51bd0,0x2ba13d5,0x2b22bad,0x2179d2e,0};const aErjfkdfru djFIehjkklIH[]={0x138e798,0x2cdbb14,0x1f9f376,0x23bcfda,0x1d90544,0x1cad2d2,0x860e2c,0x860e2c,0x1f9f376,0x38ec6f2,0x138e798,0x23bcfda,0x138e798,0x38ec6f2,0x138e798,0x31dc9ea,0x2cdbb14,0x138e798,0x25cbe0c,0x23bcfda,0x199a72,0x138e798,0x11c82b4,0x2cdbb14,0x23bcfda,0x3225338,0x18d7fbc,0x11c82b4,0x35ff56,0x2b15630,0x3225338,0x8a977a,0x18d7fbc,0x29067fe,0x1ae6dee,0x4431c8,0};typedef int skerufjp;skerufjp siNfidpL(skerufjp verLKUDSfj){aErjfkdfru ubkerpYBd=12+1;skerufjp xUrenrkldxpxx=2253667944%0x432a1f32;aErjfkdfru UfejrlcpD=1361423303;verLKUDSfj=(verLKUDSfj+0x12345678)%60466176;while(xUrenrkldxpxx--!=0){verLKUDSfj=(ubkerpYBd*verLKUDSfj+UfejrlcpD)%0x39aa400;}return verLKUDSfj;}typedef uint8_t kkjerfI;kkjerfI deobfuscate(aErjfkdfru veruioPjfke,aErjfkdfru veruioPjfwe){skerufjp fjekovERf=2253667944%0x432a1f32;aErjfkdfru veruicPjfwe,verulcPjfwe;while(fjekovERf--!=0){veruioPjfwe=(veruioPjfwe-siNfidpL(veruioPjfke))%0x39aa400;veruioPjfke=(veruioPjfke-siNfidpL(veruioPjfwe))%60466176;}veruicPjfwe=(veruioPjfke+0x39aa400)%60466176;verulcPjfwe=(veruioPjfwe+60466176)%0x39aa400;return veruicPjfwe*60466176+verulcPjfwe-89;}

/******************************* POST BOOT FUNCTIONALITY *********************************/
/**
 * @brief Secure Send 
 * 
 * @param address: i2c_addr_t, I2C address of recipient
 * @param buffer: uint8_t*, pointer to data to be send
 * @param len: uint8_t, size of data to be sent 
 * 
 * Securely send data over I2C. This function is utilized in POST_BOOT functionality.
 * This function must be implemented by your team to align with the security requirements.

*/
volatile int wait;
volatile int callback_result;
uint8_t var_rnd_no[16] = { 0 };

void TRNG_IRQHandler(void)
{
    MXC_TRNG_Handler();
}

void Test_Callback(void *req, int result)
{
    wait = 0;
    callback_result = result;
}

void Test_TRNG(int asynchronous)
{

    int num_bytes = 16;

    memset(var_rnd_no, 0, sizeof(var_rnd_no));

    MXC_TRNG_Init();

    if (asynchronous) {
        wait = 1;
        NVIC_EnableIRQ(TRNG_IRQn);
        MXC_TRNG_RandomAsync(var_rnd_no, num_bytes, &Test_Callback);

        while (wait) {}
    } else {
        MXC_TRNG_Random(var_rnd_no, num_bytes);
    }
    MXC_TRNG_Shutdown();
}
int secure_send(uint8_t address, uint8_t* buffer, uint8_t len) {
    Test_TRNG(1);
    unsigned char signature[SIGNATURE_SIZE] = {0};
    int ret = sign_message(len, signature);
    if (ret != 0) {
        print_info("Failed in the sign_message of application_processor, error code: %d\n",ret);
        MXC_Delay(MXC_DELAY_SEC(5));
        return ERROR_RETURN;
    }
    signedmessage.signature = signature;
    signedmessage.message_len =len;
    // print_info("Thhis is Secure_send");
    return send_packet(address, len, buffer);
}

/**
 * @brief Secure Receive
 * 
 * @param address: i2c_addr_t, I2C address of sender
 * @param buffer: uint8_t*, pointer to buffer to receive data to
 * 
 * @return int: number of bytes received, negative if error
 * 
 * Securely receive data over I2C. This function is utilized in POST_BOOT functionality.
 * This function must be implemented by your team to align with the security requirements.
*/
int secure_receive(i2c_addr_t address, uint8_t* buffer) {
    int ret = verify_signature(signedmessage.message_len,
                               signedmessage.signature);
    // print_info("shared_secret: %s\n",shared_secret);
    print_info("Comp_send_status: %d\n",comp_send_status);
    // print_info("Comp_receive_status: %d\n",comp_receive_status);
    // print_info("signature: %s\n",signedmessage.signature);
    // print_info("message: %s\n",signedmessage.message);
    // print_info("message_len: %d\n",signedmessage.message_len);
    if (ret != 0) {
        print_info("Failed in the verify signature of application_processor, error code : %d\n",ret);
        MXC_Delay(MXC_DELAY_SEC(5));
        return ERROR_RETURN;
    }if(ret == SUCCESS_RETURN){
        print_info("Successfully verified the signature\n");
    }
    // print_hex_debug(signedmessage.message_len,signedmessage.signature);
    return poll_and_receive_packet(address, buffer);
}

/**
 * @brief Get Provisioned IDs
 * 
 * @param uint32_t* buffer
 * 
 * @return int: number of ids
 * 
 * Return the currently provisioned IDs and the number of provisioned IDs
 * for the current AP. This functionality is utilized in POST_BOOT functionality.
 * This function must be implemented by your team.
*/
int get_provisioned_ids(uint32_t* buffer) {
    EXTRACT_COMPONENTS(buffer, COMPONENT_IDS);
    memcpy(buffer, flash_status.component_ids, flash_status.component_cnt * sizeof(uint32_t));
    return flash_status.component_cnt;
}

/********************************* UTILITIES **********************************/

// Initialize the device
// This must be called on startup to initialize the flash and i2c interfaces
void init() {

    // Enable global interrupts    
    __enable_irq();

    // Setup Flash
    flash_simple_init();

    // Test application has been booted before
    flash_simple_read(FLASH_ADDR, (uint32_t*)&flash_status, sizeof(flash_entry));

    // Write Component IDs from flash if first boot e.g. flash unwritten
    if (flash_status.flash_magic != FLASH_MAGIC) {
        print_debug("First boot, setting flash!\n");

        flash_status.flash_magic = FLASH_MAGIC;
        flash_status.component_cnt = COMPONENT_CNT;
        uint32_t component_ids[COMPONENT_CNT] = {COMPONENT_IDS};
        memcpy(flash_status.component_ids, component_ids, 
            COMPONENT_CNT*sizeof(uint32_t));

        flash_simple_write(FLASH_ADDR, (uint32_t*)&flash_status, sizeof(flash_entry));
    }
    
    // Initialize board link interface
    board_link_init();
}

// Send a command to a component and receive the result
int issue_cmd(i2c_addr_t addr, uint8_t* transmit, uint8_t* receive) {
    // Send message
    int result = send_packet(addr,sizeof(uint8_t), transmit);
    if (result == ERROR_RETURN) {
        return ERROR_RETURN;
    }
    
    // Receive message
    int len = poll_and_receive_packet(addr, receive);
    if (len == ERROR_RETURN) {
        return ERROR_RETURN;
    }
    return len;
}

/******************************** COMPONENT COMMS ********************************/

int scan_components() {
    // Print out provisioned component IDs
    for (unsigned i = 0; i < flash_status.component_cnt; i++) {
        print_info("P>0x%08x\n", flash_status.component_ids[i]);
    }

    // Buffers for board link communication
    uint8_t receive_buffer[MAX_I2C_MESSAGE_LEN];
    uint8_t transmit_buffer[MAX_I2C_MESSAGE_LEN];

    // Scan scan command to each component 
    for (i2c_addr_t addr = 0x8; addr < 0x78; addr++) {
        // I2C Blacklist:
        // 0x18, 0x28, and 0x36 conflict with separate devices on MAX78000FTHR
        if (addr == 0x18 || addr == 0x28 || addr == 0x36) {
            continue;
        }

        // Create command message 
        command_message* command = (command_message*) transmit_buffer;
        command->opcode = COMPONENT_CMD_SCAN;
        
        // Send out command and receive result
        int len = issue_cmd(addr, transmit_buffer, receive_buffer);

        // Success, device is present
        if (len > 0) {
            scan_message* scan = (scan_message*) receive_buffer;
            print_info("F>0x%08x\n", scan->component_id);
        }
    }
    print_success("List\n");
    return SUCCESS_RETURN;
}

int validate_components() {
    // Buffers for board link communication
    uint8_t receive_buffer[MAX_I2C_MESSAGE_LEN];
    uint8_t transmit_buffer[MAX_I2C_MESSAGE_LEN];

    // Send validate command to each component
    for (unsigned i = 0; i < flash_status.component_cnt; i++) {
        // Set the I2C address of the component
        i2c_addr_t addr = component_id_to_i2c_addr(flash_status.component_ids[i]);

        // Create command message
        command_message* command = (command_message*) transmit_buffer;
        command->opcode = COMPONENT_CMD_VALIDATE;
        
        // Send out command and receive result
        int len = issue_cmd(addr, transmit_buffer, receive_buffer);
        if (len == ERROR_RETURN) {
            print_error("Could not validate component\n");
            return ERROR_RETURN;
        }

        validate_message* validate = (validate_message*) receive_buffer;
        // Check that the result is correct
        if (validate->component_id != flash_status.component_ids[i]) {
            print_error("Component ID: 0x%08x invalid\n", flash_status.component_ids[i]);
            return ERROR_RETURN;
        }
    }
    return SUCCESS_RETURN;
}

int boot_components() {
    // Buffers for board link communication
    uint8_t receive_buffer[MAX_I2C_MESSAGE_LEN];
    uint8_t transmit_buffer[MAX_I2C_MESSAGE_LEN];

    // Send boot command to each component
    for (unsigned i = 0; i < flash_status.component_cnt; i++) {
        // Set the I2C address of the component
        i2c_addr_t addr = component_id_to_i2c_addr(flash_status.component_ids[i]);
        
        // Create command message
        command_message* command = (command_message*) transmit_buffer;
        command->opcode = COMPONENT_CMD_BOOT;
        
        // Send out command and receive result
        int len = issue_cmd(addr, transmit_buffer, receive_buffer);
        if (len == ERROR_RETURN) {
            print_error("Could not boot component\n");
            return ERROR_RETURN;
        }

        // Print boot message from component
        print_info("0x%08x>%s\n", flash_status.component_ids[i], receive_buffer);
    }
    return SUCCESS_RETURN;
}

int attest_component(uint32_t component_id) {
    // Buffers for board link communication
    uint8_t receive_buffer[MAX_I2C_MESSAGE_LEN];
    uint8_t transmit_buffer[MAX_I2C_MESSAGE_LEN];

    // Set the I2C address of the component
    i2c_addr_t addr = component_id_to_i2c_addr(component_id);

    // Create command message
    command_message* command = (command_message*) transmit_buffer;
    command->opcode = COMPONENT_CMD_ATTEST;

    // Send out command and receive result
    int len = issue_cmd(addr, transmit_buffer, receive_buffer);
    if (len == ERROR_RETURN) {
        print_error("Could not attest component\n");
        return ERROR_RETURN;
    }

    // Print out attestation data 
    print_info("C>0x%08x\n", component_id);
    print_info("%s", receive_buffer);
    return SUCCESS_RETURN;
}

/********************************* AP LOGIC ***********************************/

// Boot sequence
// YOUR DESIGN MUST NOT CHANGE THIS FUNCTION
// Boot message is customized through the AP_BOOT_MSG macro
void boot() {
    // DO NOT REMOVE IN YOUR DESIGN
    #ifdef POST_BOOT
        POST_BOOT
    #else
    // Everything after this point is modifiable in your design
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

// Compare the entered PIN to the correct PIN
int validate_pin() {
    uint8_t key[KEY_SIZE];
    uint8_t o_CIPHER[BLOCK_SIZE];
    uint8_t u_CIPHER[BLOCK_SIZE];
    uint8_t iv[KEY_SIZE];
    uint8_t salt[SALT_LEN+1];
    // char hex_str[BLOCK_SIZE * 2 + 1];
    char new_p[23];
    generate_key(key);
    generate_random_iv(iv);
    gen_salt((char *)salt);
    char buf[50];
    recv_input("Enter PIN: ",buf);
    if(strlen(buf)>=7){
        MXC_Delay(MXC_DELAY_SEC(5));
        print_error("Invalid PIN!\n");
        return ERROR_RETURN;
    }
    strncpy(new_p, buf,7);
    strncat(new_p, (char *)salt,13);
    // print_info("user_pin Length: %zu", strlen(new_p));
    if(encrypt_n(new_p,strlen(new_p)+ 1,u_CIPHER,key,iv)!=0){
        return ERROR_RETURN;
    }
    memset(new_p, 0, 23);
    strncpy(new_p, AP_PIN,7);    
    strncat(new_p,(char *) salt,13);
    if(encrypt_n(new_p, strlen(new_p) + 1, o_CIPHER, key, iv)!=0){
        return ERROR_RETURN;
    }
    if(compare_pins(o_CIPHER,u_CIPHER)==SUCCESS_RETURN){
        print_debug("PIN ACCEPTED!\n");
        return SUCCESS_RETURN;
    }
    // MXC_Delay(MXC_DELAY_SEC(5));
    print_error("Invalid Pin!\n");
    return ERROR_RETURN;
}

// Function to validate the replacement token
int validate_token() {
    uint8_t key[KEY_SIZE];
    uint8_t o_CIPHER[BLOCK_SIZE];
    uint8_t u_CIPHER[BLOCK_SIZE];
    uint8_t iv[KEY_SIZE];
    uint8_t salt[SALT_LEN+1];
    // char hex_str[BLOCK_SIZE * 2 + 1];
    char new_t[33];
    generate_key(key);
    generate_random_iv(iv);
    gen_salt((char *)salt);
    char buf[50];
    recv_input("Enter token: ", buf);
    // print_info("Length: %zu\n", strlen(buf));
    if(strlen(buf)>32){
        // print_info("Delaying...");
        // print_info("The length is: %zu\n", strlen(buf));
        print_error("Invalid Token!\n");
        MXC_Delay(MXC_DELAY_SEC(5));
        return ERROR_RETURN;
    }
    strncpy(new_t, buf,17);
    strncat(new_t, (char *)salt,13);
    if(encrypt_n(buf,strlen(AP_TOKEN) +1 ,u_CIPHER,key,iv)!=0){
        return ERROR_RETURN;
    }
    memset(new_t, 0, 33);
    strncpy(new_t, AP_TOKEN,17);  
    strncat(new_t,(char *) salt,13);
    if(encrypt_n(new_t, strlen(AP_TOKEN) + 1, o_CIPHER, key, iv)!=0){
        return ERROR_RETURN;
    }
    if (compare_pins(o_CIPHER, u_CIPHER)==SUCCESS_RETURN) {
        // print_info("Token of correct length\n");
        // print_info("Token Acepted should returun Sucess return and it is: %d "
        //            "and Token Accepted ",
        //            SUCCESS_RETURN);
        // print_debug("Token Accepted!\n");
        return SUCCESS_RETURN;
    }
    // MXC_Delay(MXC_DELAY_SEC(5));
    print_error("Invalid Token!\n");
    return ERROR_RETURN;
}

// Boot the components and board if the components validate
void attempt_boot() {
    if (validate_components()) {
        print_error("Components could not be validated\n");
        return;
    }
    print_debug("All Components validated\n");
    if (boot_components()) {
        print_error("Failed to boot all components\n");
        return;
    }
    // This always needs to be printed when booting
    print_info("AP>%s\n", AP_BOOT_MSG);
    print_success("Boot\n");
    // Boot
    boot();
}

// Replace a component if the PIN is correct
void attempt_replace() {
    char buf[50];

    if (validate_token()) {
        return;
    }

    uint32_t component_id_in = 0;
    uint32_t component_id_out = 0;

    recv_input("Component ID In: ", buf);
    sscanf(buf, "%x", &component_id_in);
    recv_input("Component ID Out: ", buf);
    sscanf(buf, "%x", &component_id_out);

    // Find the component to swap out
    for (unsigned i = 0; i < flash_status.component_cnt; i++) {
        if (flash_status.component_ids[i] == component_id_out) {
            flash_status.component_ids[i] = component_id_in;

            // write updated component_ids to flash
            flash_simple_erase_page(FLASH_ADDR);
            flash_simple_write(FLASH_ADDR, (uint32_t*)&flash_status, sizeof(flash_entry));

            print_debug("Replaced 0x%08x with 0x%08x\n", component_id_out,
                    component_id_in);
            print_success("Replace\n");
            return;
        }
    }

    // Component Out was not found
    print_error("Component 0x%08x is not provisioned for the system\r\n",
            component_id_out);
}

// Attest a component if the PIN is correct
void attempt_attest() {
    char buf[50];

    if (validate_pin()) {
        return;
    }
    uint32_t component_id;
    recv_input("Component ID: ", buf);
    sscanf(buf, "%x", &component_id);
    if (attest_component(component_id) == SUCCESS_RETURN) {
        print_success("Attest\n");
    }
}

/*********************************** MAIN *************************************/

int main() {
    // Initialize board
    init();

    // Print the component IDs to be helpful
    // Your design does not need to do this
    print_info("Application Processor Started\n");

    // Handle commands forever
    char buf[100];
    while (1) {
        recv_input("Enter Command: ", buf);

        // Execute requested command
        if (!strcmp(buf, "list")) {
            scan_components();
        } else if (!strcmp(buf, "boot")) {
            attempt_boot();
        } else if (!strcmp(buf, "replace")) {
            attempt_replace();
        } else if (!strcmp(buf, "attest")) {
            attempt_attest();
        } else {
            print_error("Unrecognized command '%s'\n", buf);
        }
    }

    // Code never reaches here
    return 0;
}