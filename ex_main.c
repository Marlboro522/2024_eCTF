#include<stdio.h>
#include "ex_key.h"
int main(){
    uint8_t data[10] = "Hello";
    uint8_t signature[SIGNATURE_SIZE];
    initialize_key();
    sign(data, 5, signature);
    printf("%s", signature);
    printf("\n%s", data);
    int result = sign_veriffy(data, 5, signature);
    printf("Result: %d\n", result);
    return 0;
}