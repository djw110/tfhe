#include "tlwe.h"

typedef struct {
    uint32_t *a;
    uint32_t *b;
} poly_cipher;


void poly_cipher_init(poly_cipher *c);
void poly_cipher_free(poly_cipher *c);
void poly_encrypt(poly_cipher *c, uint8_t *s, uint32_t *m);
void poly_decrypt(uint32_t *buf, poly_cipher *c, uint8_t *s);
void poly_bin_encrypt(poly_cipher *c, uint8_t *s, uint8_t *m);
void poly_bin_decrypt(uint8_t *buf, poly_cipher *c, uint8_t *s);
void poly_add_h(poly_cipher *out, poly_cipher *c1, poly_cipher *c2);
void poly_add_c(poly_cipher *out, poly_cipher *c, uint8_t *m);
void poly_negate(poly_cipher *out, poly_cipher *c);