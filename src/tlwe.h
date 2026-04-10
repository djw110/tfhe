#include "secure_sample.h"

typedef struct {
    uint32_t *a;
    uint32_t b;
} ciphertext;

void cipher_init(ciphertext *c);
void cipher_free(ciphertext *c);
void gen_sk(uint8_t *buf);
void encrypt(ciphertext *c, uint8_t *s, uint8_t m);
uint8_t decrypt(uint8_t *s, ciphertext *c);
void lwe_add_h(ciphertext *out, ciphertext *c1, ciphertext *c2);
void lwe_add_c(ciphertext *out, ciphertext *c, uint8_t m);
void lwe_negate(ciphertext *out, ciphertext *c);