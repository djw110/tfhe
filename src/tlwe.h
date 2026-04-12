#include "secure_sample.h"

typedef struct {
    uint32_t *a;
    uint32_t b;
} std_cipher;

void std_cipher_init(std_cipher *c);
void std_cipher_free(std_cipher *c);
void gen_sk(uint8_t *buf);
void std_encrypt(std_cipher *c, uint8_t *s, uint8_t m);
uint8_t std_decrypt(uint8_t *s, std_cipher *c);
void std_add_h(std_cipher *out, std_cipher *c1, std_cipher *c2);
void std_add_c(std_cipher *out, std_cipher *c, uint8_t m);
void std_negate(std_cipher *out, std_cipher *c);