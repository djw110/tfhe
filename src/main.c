#include "trlwe.h"

void test_lwe_encrypt(size_t numtests);

int main(int argc, char* argv[]){
    uint8_t s[n];
    std_cipher c1;
    std_cipher c2;
    std_cipher temp;
    std_gen_sk(s);
    std_cipher_init(&c1);
    std_cipher_init(&c2);
    std_cipher_init(&temp);

    







    std_cipher_free(&c1);
    std_cipher_free(&c2);
    std_cipher_free(&temp);
    return 0;
}

void test_lwe_encrypt(size_t numtests){
    uint8_t s[n];
    std_cipher c;
    std_gen_sk(s);
    std_cipher_init(&c);

    int error = 0;
    for (int count = 0; count < numtests; count++){
        for (int i = 0; i < 2; i++){
            std_encrypt(&c,s,i);
            if (i != std_decrypt(s,&c)){
                error += 1;
            }
        }
    }
    std_cipher_free(&c);
    printf("Errs: %d\n",error);
}