#include "tgrsw.h"
#include <string.h>

void gsw_cipher_init(gsw_cipher *c){
    c->rows = malloc(2 * l * sizeof(poly_cipher));

    for (int i = 0; i < 2 * l; i++){
        poly_cipher_init(&(c->rows[i]));
    }
}

void gsw_cipher_free(gsw_cipher *c){
    for (int i = 0; i < 2*l; i++){
        poly_cipher_free(&(c->rows[i]));
    }
    
    free(c->rows);
    c->rows = NULL;
}

// TODO: Review how level + baselog collaborate
void poly_gadget_scale(uint32_t *m, uint32_t i){
    uint32_t shift = 32 - (i + 1) * base_log;
    for (int k = 0; k < n; k++){
        m[k] = m[k] << shift;
    }
}

void gsw_encrypt(gsw_cipher *c, uint8_t *s, uint32_t *m){
    uint32_t zeroes[n];
    uint32_t m_scaled[n];
    for (int j = 0; j < n; j++){
        zeroes[j] = 0;
    }
    for (int i = 0; i < l; i++){
        memcpy(m_scaled, m, sizeof(uint32_t) * n);    
        poly_gadget_scale(m_scaled, i);

        poly_encrypt(&(c->rows[i]), s, zeroes);
        for (int j=0; j < n; j++){
            c->rows[i].a[j] -= m_scaled[j]; 
        }

        poly_encrypt(&(c->rows[i+l]), s, zeroes);
        for (int j=0; j < n; j++){
            c->rows[i+l].b[j] += m_scaled[j]; 
        }
    }
}