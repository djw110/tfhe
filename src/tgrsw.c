#include "tgrsw.h"

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