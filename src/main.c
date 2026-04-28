#include "tgrsw.h"

int test_lwe_encrypt(size_t numtests);
int test_lwe_add(size_t numtests);
int test_rlwe_encrypt(size_t numtests);
int test_rlwe_add(size_t numtests);

int main(int argc, char* argv[]){
    printf("LWE Encryption Errs: %d\n", test_lwe_encrypt(3));
    printf("LWE XOR Errs: %d\n", test_lwe_add(3));
    printf("RLWE Encryption Errs: %d\n", test_rlwe_encrypt(3));
    printf("RLWE XOR Errs: %d\n", test_rlwe_add(3));
    return 0;
}

int test_lwe_encrypt(size_t numtests){
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
    return error;
}

int test_lwe_add(size_t numtests){
    uint8_t s[n];
    gen_sk(s);
    std_cipher c1;
    std_cipher c2;
    std_cipher cout;
    std_cipher_init(&c1);
    std_cipher_init(&c2);
    std_cipher_init(&cout);

    int error = 0;
    for (int count = 0; count < numtests; count++){
        uint8_t m1 = sec_rand_bin();
        uint8_t m2 = sec_rand_bin();

        std_encrypt(&c1, s, m1);
        std_encrypt(&c2, s, m2);
        std_add_h(&cout, &c1, &c2);
        if (m1 ^ m2 != std_decrypt(s,&cout)){
            error++;
        }
    }
    std_cipher_free(&c1); std_cipher_free(&c2); std_cipher_free(&cout);
    return error;
}

int test_rlwe_encrypt(size_t numtests){
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
    poly_cipher_free(&c);
    free(testpoly); free(buf);

    return error;
}

int test_rlwe_add(size_t numtests){
    uint8_t s[n];
    gen_sk(s);
    poly_cipher c1;
    poly_cipher c2;
    poly_cipher cout;
    poly_cipher_init(&c1);
    poly_cipher_init(&c2);
    poly_cipher_init(&cout);

    uint8_t *testpoly1 = malloc(n * sizeof(uint8_t));
    uint8_t *testpoly2 = malloc(n * sizeof(uint8_t));
    uint8_t *buf1 = malloc(n * sizeof(uint8_t));
    uint8_t *buf2 = malloc(n * sizeof(uint8_t));

    int error = 0;
    for (int count = 0; count < numtests; count++){
        for(int i = 0; i < n; i ++){
            testpoly1[i] = sec_rand_bin();
            testpoly2[i] = sec_rand_bin();
        }
        poly_bin_encrypt(&c1, s, testpoly1);
        poly_bin_encrypt(&c2, s, testpoly2);
        poly_add_h(&cout, &c1, &c2);
        poly_bin_decrypt(buf1, &cout, s);
        bin_poly_xor(testpoly1, testpoly2, buf2);
        for (int i = 0; i < n; i++){
            if (buf1[i] != buf2[i]){
                error += 1;
            }
        }
    }
    poly_cipher_free(&c1); poly_cipher_free(&c2); poly_cipher_free(&cout);
    free(testpoly1); free(testpoly2); free(buf1); free(buf2);

    return error;
}

int test_ext_product(size_t numtests){
    uint8_t s[n];
    gen_sk(s);
    poly_cipher rlwe;
    poly_cipher output;
    gsw_cipher gsw;
    int error = 0;
    for (int j = 0; j < numtests; j++){
        int l_error = 0;
        poly_cipher_init(&rlwe);
        poly_cipher_init(&output);
        gsw_cipher_init(&gsw);

        uint32_t *testpoly_1 = malloc(n*sizeof(uint32_t));
        uint32_t *testpoly_2 = malloc(n*sizeof(uint32_t));
        uint32_t *buf_1 = malloc(n * sizeof(uint32_t));
        uint32_t *buf_2 = malloc(n * sizeof(uint32_t));

        for(int i = 0; i < n; i++){
            testpoly_1[i] = 0;
            testpoly_2[i] = 0;
        }
        testpoly_1[0] = TORUS_0_25;
        testpoly_2[0] = TORUS_0_25;

        poly_encrypt(&rlwe, s, testpoly_1);
        gsw_encrypt(&gsw, s, testpoly_2);
        external_product(&rlwe, &gsw, &output);
        poly_decrypt(buf_1, &output, s);
        ring_poly_mul(testpoly_1, testpoly_2, buf_2);

        for (int i = 0; i < n; i ++){
            uint32_t d = buf_1[i] - buf_2[i];
            uint32_t dist = (d > (UINT32_MAX / 2)) ? (UINT32_MAX - d) : d;
            if (dist > 0x10000) {
                l_error++;
            }
        }

        free(testpoly_1);
        free(testpoly_2);
        free(buf_1);
        free(buf_2);
        poly_cipher_free(&rlwe);
        poly_cipher_free(&output);
        gsw_cipher_free(&gsw);

        if (l_error){
            error++;
        }
    }
    return error;
}