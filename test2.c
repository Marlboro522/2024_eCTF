#include <stdio.h>
#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/aes.h>

int main(void) {
Aes enc;
Aes dec;
int ret;

int x;

byte key[] = {"234567890abcdef"};

byte iv[]  = {"1234567890abcdef"};

byte plain[32]  = {"Hola mundoHola mundoHola mundo"};   // an increment of 16, fill with data
byte plain2[32];
byte cipher[32];

printf("sizeof key = %d\n", (int) sizeof(key));
printf("sizeof cipher = %d\n", (int) sizeof(cipher));
printf("sizeof plain = %d\n", (int) sizeof(plain));

printf("plain value                      ");

for(x=0; x<16; x++){
  printf("%02x", plain[x]);
  printf(" ");
  }
printf(" \n");



printf("printing key value               ");

for(x=0; x<16; x++){
  printf("%02x", key[x]);
  printf(" ");
  }
printf(" \n");


printf("printing iv value                ");
for(x=0; x<16; x++){
  printf("%02x", iv[x]);
  printf(" ");
  }

printf(" \n");

//encrypt
ret = wc_AesSetKey(&enc, key, sizeof(key), iv, AES_ENCRYPTION);
if (ret != 0) {
    printf("Encrypt set key failed with error: %d\n", ret);
    return ret;
}
ret = wc_AesCbcEncrypt(&enc, cipher, plain, sizeof(plain));
if (ret != 0) {
    printf("AesCbcEncrypt failed with error: %d\n", ret);
    return ret;
}



printf("cipher value                     ");

for(x=0; x<16; x++){
  printf("%02x", cipher[x]);
  printf(" ");
  }
printf(" \n");



printf("printing key value before cipher ");

for(x=0; x<16; x++){
  printf("%02x", key[x]);
  printf(" ");
  }
printf(" \n");


printf("printing iv value before cipher  ");
for(x=0; x<16; x++){
  printf("%02x", iv[x]);
  printf(" ");
  }
printf(" \n");

//decrypt
ret = wc_AesSetKey(&dec, key, sizeof(key), iv, AES_DECRYPTION);
if (ret != 0) {
    printf("Decrypt set key failed with error: %d\n", ret);
    return ret;
}
ret = wc_AesCbcDecrypt(&dec, plain2, cipher, sizeof(cipher));
if (ret != 0) {
    printf("AesCbcDecrypt failed with error: %d\n", ret);
    return ret;
}


printf("plain2 value                     ");

for(x=0; x<16; x++){
  printf("%02x", plain2[x]);
  printf(" ");
  }

printf(" \n");
printf(" \n");
printf(" \n");

}