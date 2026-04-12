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

void poly_bin_encrypt(poly_cipher *c, uint8_t *s, uint8_t *m){
    uint32_t new_m[n];
    for(int i = 0; i < n; i++){
        new_m[i] = (m[i] == 1) ? TORUS_0_25 : 0;
    }
    poly_encrypt(c, s, new_m);
}

void poly_decrypt(uint32_t *phase, poly_cipher *c, uint8_t *s){
    for (int k=0; k<n; k++){
        phase[k] = c->b[k];
    }

    for (int i = 0; i < n; i++){
        if (s[i] == 1){
            for (int j=0; j < n; j++){
                if (i+j < n){
                    phase[i+j] -= c->a[j];
                }
                else {
                    phase[i+j-n] += c->a[j];
                }
            }
        }
    }

    
}

void poly_bin_decrypt(uint8_t *buf, poly_cipher *c, uint8_t *s){
    uint32_t phase[n];
    poly_decrypt(phase, c, s);
    for (int k = 0; k<n; k++){
        if (phase[k] > TORUS_0_125 && phase[k] < TORUS_0_375){
            buf[k] = 1;
        }
        else {
            buf[k] = 0;
        }
    }
}

void poly_add_h(poly_cipher *out, poly_cipher *c1, poly_cipher *c2){
    for (int i = 0; i < n; i++){
        out->a[i] = c1->a[i] + c2->a[i];
        out->b[i] = c1->b[i] + c2->b[i];
    }
}

void poly_add_c(poly_cipher *out, poly_cipher *c, uint8_t *m){
    for (int i = 0; i < n; i++){
        uint32_t scaled_num = (m[i]==1) ? TORUS_0_25 : 0;
        out->a[i] = c->a[i];
        out->b[i] = c->b[i] + scaled_num;
    }
}

void poly_negate(poly_cipher *out, poly_cipher *c){
    for (int i = 0; i < n; i++){
        out->a[i] = 0 - c->a[i];
        out->b[i] = 0 - c->b[i];
    }
}
