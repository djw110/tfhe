#include "tlwe.h"

void test_lwe_encrypt(size_t numtests);

int main(int argc, char* argv[]){
    uint8_t s[n];
    ciphertext c1;
    ciphertext c2;
    ciphertext temp;
    gen_sk(s);
    cipher_init(&c1);
    cipher_init(&c2);
    cipher_init(&temp);



    cipher_free(&c1);
    cipher_free(&c2);
    cipher_free(&temp);
    return 0;
}

void test_lwe_encrypt(size_t numtests){
    uint8_t s[n];
    ciphertext c;
    gen_sk(s);
    cipher_init(&c);

    int error = 0;
    for (int count = 0; count < numtests; count++){
        for (int i = 0; i < 2; i++){
            encrypt(&c,s,i);
            if (i != decrypt(s,&c)){
                error += 1;
            }
        }
    }
    cipher_free(&c);
    printf("Errs: %d\n",error);
}