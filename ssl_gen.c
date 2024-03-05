#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/ecc.h>
#include <stdio.h>

int main() {
    // Initialize WolfSSL
    wolfSSL_Init();

    // Initialize RNG
    WC_RNG rng;
    if (wc_InitRng(&rng) != 0) {
        printf("Error initializing RNG\n");
        return 1;
    }

    // Generate ECC key pair
    ecc_key key;
    if (ecc_make_key(&rng, ECC_SECP256R1, 0, &key) != 0) {
        printf("Error generating ECC key pair\n");
        return 1;
    }

    // Create X509 certificate
    WOLFSSL_X509* cert = wolfSSL_X509_new();
    if (cert == NULL) {
        printf("Error creating certificate\n");
        return 1;
    }

    // Set certificate subject and issuer information
    // (Replace with actual subject and issuer information)
    wolfSSL_X509_set_subject_name(cert, "CN=TLS");
    wolfSSL_X509_set_issuer_name(cert, "CN=Issuer");

    // Set certificate public key (dummy key for example purposes)
    byte dummy_pub_key[] = {0x01, 0x02, 0x03, 0x04}; // Replace with actual key
    wolfSSL_X509_set_pubkey(cert, dummy_pub_key, sizeof(dummy_pub_key));

    // Set certificate validity period (forever)
    wolfSSL_X509_set_lifetime(cert, -1, -1);

    // Sign certificate with private key
    if (wolfSSL_X509_set_serial(cert, 1) != 1 ||
        wolfSSL_X509_self_sign(cert, &key) != 1) {
        printf("Error signing certificate\n");
        return 1;
    }

    // Save certificate and private key to files
    // (Replace with actual file paths)
    FILE* cert_file = fopen("/Users/ilu/courseWork/2024_eCTF/cert.pem", "w");
    if (cert_file == NULL) {
        printf("Error opening certificate file\n");
        return 1;
    }
    wolfSSL_PEM_write_X509(cert_file, cert);
    fclose(cert_file);

    FILE* key_file = fopen("/Users/ilu/courseWork/2024_eCTF/private_key.pem", "w");
    if (key_file == NULL) {
        printf("Error opening private key file\n");
        return 1;
    }
    wolfSSL_PEM_write_key(key_file, &key);
    fclose(key_file);

    // Cleanup
    wolfSSL_X509_free(cert);
    wc_FreeRng(&rng);
    wolfSSL_Cleanup();

    printf("Keys and certificate generated successfully\n");

    return 0;
}
