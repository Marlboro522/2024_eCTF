#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <mxc_device.h>
#include <nvic_table.h>
#include <trng.h>

volatile int wait;
volatile int callback_result;

/***** Globals *****/
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

void print(char *stuff)
{
    int i, j, size = 4;

    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            printf("0x%02x ", stuff[i * size + j]);
        }

        printf("\n");
    }

    return;
}