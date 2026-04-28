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

void poly_gadget_scale(uint32_t *m, uint32_t i){
    uint32_t shift = 32 - (i + 1) * base_log;
    for (int k = 0; k < n; k++){
        m[k] = m[k] << shift;
    }
}

// TODO: Ensure this is proper inverse of poly_gadget_scale()
void poly_gadget_decompose(uint32_t *poly_in, uint32_t **out){
    uint32_t mask = (1U << base_log) - 1;
    uint32_t half_base = (1U << (base_log - 1));

    for (int i = 0; i < n; i++){
        uint32_t val = poly_in[i];

        for (int j = 1; j <= l; j++) {
            int shift = 32 - (j * base_log);
            uint32_t digit = (val >> shift) & mask;
            out[j-1][i] = digit;
        }
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
            c->rows[i].a[j] += m_scaled[j]; 
        }

        poly_encrypt(&(c->rows[i+l]), s, zeroes);
        for (int j=0; j < n; j++){
            c->rows[i+l].b[j] += m_scaled[j]; 
        }
    }
}

// TODO: Logical improvements. Does not always produce true product.
void external_product(poly_cipher *cin, gsw_cipher *min, poly_cipher *cout){
    poly_cipher_zero(cout);
    uint32_t **dec_a = malloc (l * sizeof(uint32_t*));
    uint32_t **dec_b = malloc (l * sizeof(uint32_t*));
    uint32_t *temp_a = malloc(n * sizeof(uint32_t));
    uint32_t *temp_b = malloc(n * sizeof(uint32_t)); 
    for (int i = 0; i < l; i++){
        dec_a[i] = malloc(n * sizeof(uint32_t));
        dec_b[i] = malloc(n * sizeof(uint32_t));
    }
    poly_gadget_decompose(cin->a, dec_a);
    poly_gadget_decompose(cin->b, dec_b);

    for (int i = 0; i < l; i++){      
        
        // Handle decomposed 'a'
        poly_cipher *row = &min->rows[(i)];
        ring_poly_mul(dec_a[i], row->a, temp_a);
        ring_poly_mul(dec_a[i], row->b, temp_b);

        ring_poly_add(cout->a, temp_a, cout->a);
        ring_poly_add(cout->b, temp_b, cout->b);

        // Handle decomposed 'b'
        row = &min->rows[(i+l)];
        ring_poly_mul(dec_b[i], row->a, temp_a);
        ring_poly_mul(dec_b[i], row->b, temp_b);

        ring_poly_add(cout->a, temp_a, cout->a);
        ring_poly_add(cout->b, temp_b, cout->b);
    }

    for (int i = 0; i < l; i++){
        free(dec_a[i]);
        free(dec_b[i]);
    }
    free(dec_a);
    free(dec_b);
    free(temp_a);
    free(temp_b);
}