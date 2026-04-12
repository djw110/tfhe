#include "tgrsw.h"

void test_lwe_encrypt(size_t numtests);
void test_rlwe_encrypt(size_t numtests);

int main(int argc, char* argv[]){
    test_rlwe_encrypt(3);
    return 0;
}

void test_lwe_encrypt(size_t numtests){
    uint8_t s[n];
    gen_sk(s);
    std_cipher c;
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

void test_rlwe_encrypt(size_t numtests){
    uint8_t s[n];
    gen_sk(s);
    poly_cipher c;
    poly_cipher_init(&c);

    uint8_t *testpoly = malloc(n * sizeof(uint8_t));
    uint8_t *buf = malloc(n * sizeof(uint8_t));

    int error = 0;
    for (int count = 0; count < numtests; count++){
        for(int i = 0; i < n; i ++){
            testpoly[i] = sec_rand_bin();
        }
        poly_bin_encrypt(&c, s, testpoly);
        poly_bin_decrypt(buf, &c, s);
        for (int i = 0; i < n; i++){
            if (buf[i] != testpoly[i]){
                error += 1;
            }
        }
    }
    printf("Errs %d\n",error);
}