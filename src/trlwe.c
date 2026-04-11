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