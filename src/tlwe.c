#include "tlwe.h"

void std_cipher_init(std_cipher *c){
    c->a = malloc(n * sizeof(uint32_t));
}

void std_cipher_free(std_cipher *c){
    free(c->a);
    c->a = NULL;
}

void std_gen_sk(uint8_t *buf){
    for (int i=0; i<n; i++){
        buf[i] = sec_rand8();
    }
}

void gen_mask(std_cipher *c){
    for (int i = 0; i < n; i++){
        c->a[i] = sec_rand32();
    }
}

void std_add_h(std_cipher *out, std_cipher *c1, std_cipher *c2){
    for (int i = 0; i < n; i++){
        out->a[i] = c1->a[i] + c2->a[i];
    }
    out->b = c1->b + c2->b;
}

void std_add_c(std_cipher *out, std_cipher *c, uint8_t m){
    uint32_t scaled_num = (m == 1) ? TORUS_0_5 : 0;
    for (int i = 0; i < n; i ++){
        out->a[i] = c->a[i];
    }
    out->b = c->b + scaled_num;
}

void std_negate(std_cipher *out, std_cipher *c){
    for (int i = 0; i < n; i++){
        out->a[i] = 0 - c->a[i];
    }
    out->b = 0 - c->b;
}

void std_encrypt(std_cipher *c, uint8_t *s, uint8_t m){
    if (c->a == NULL){
        fprintf(stderr, "Uninitialized Cipher");
        return;
    }

    uint32_t mu = (m == 1) ? TORUS_0_5 : 0;
    uint32_t e = sec_gaussian();
    gen_mask(c);

    uint32_t dot = 0;
    for (int i = 0; i < n; i++){
        if(s[i]) dot += c->a[i];
    }

    c->b = dot + mu + e;
}

uint8_t std_decrypt(uint8_t *s, std_cipher *c){
    uint32_t phase = c->b;
    uint8_t m = 0;
    for (int i = 0; i < n; i++){
        if(s[i]) phase -= c->a[i];
    }
    if (phase > TORUS_0_25 && phase < TORUS_0_75){
        m = 1;
    }
    return m;
}