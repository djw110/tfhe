#include "trlwe.h"

void poly_cipher_init(poly_cipher *c){
    c->a = malloc(n * sizeof(uint32_t));
    c->b = malloc(n * sizeof(uint32_t));
}

void poly_cipher_free(poly_cipher *c){
    free(c->a);
    free(c->b);
    c->a = NULL;
    c->b = NULL;
}

void poly_gen_sk(uint8_t *buf){
    for (int i=0; i<n; i++){
        buf[i] = sec_rand8();
    }
}

void poly_gen_pk(poly_cipher *c){
    for (int i = 0; i < n; i++){
        c->a[i] = sec_rand32();
    }
}

// TODO: switch from ring polynomial multiplication => FFT or NTT
void poly_encrypt(poly_cipher *c, uint8_t *s, uint32_t *m){
    for (int k = 0; k < n; k++){
        c->b[k] = m[k] + sec_gaussian();
    }

    poly_gen_pk(c);

    for (int i = 0; i < n; i++){
        if (s[i] == 1){
            for (int j=0; j < n; j++){
                if (i+j < n){
                    c->b[i+j] += c->a[j]; // 1x^i * yx^j = yx^{i+j}
                }
                else {
                    c->b[i+j-n] -= c->a[j]; // if ij > n, subtract yx^{i+j-n} to account for wrap
                }
            }
        }
    }
}

void poly_decrypt(uint8_t *buf, poly_cipher *c, uint8_t *s){
    return;
}