#include "trlwe.h"

typedef struct {
    poly_cipher *rows;
} gsw_cipher;


void gsw_cipher_init(gsw_cipher *c);
void gsw_cipher_free(gsw_cipher *c);
void gsw_encrypt(gsw_cipher *c, uint8_t *s, uint32_t *m);