#include "secure_sample.h"
#include <sys/random.h>

uint32_t sec_rand32() {
    uint32_t r;
    if (getrandom(&r, sizeof(r), 0) < 0){
        return 0;
    }
    return r;
}

uint32_t sec_rand32_nozero(){
    uint32_t r = 0;
    while (r == 0){
        r = sec_rand32();
    }
}

uint8_t sec_rand_bin() {
    uint8_t r;
    if (getrandom(&r, sizeof(r), 0) < 0){
        return 0;
    }
    return r & 1;
}

// TODO: Switch to discrete gaussian to make constant-time
uint32_t sec_gaussian() {
    double u1 = (double)sec_rand32_nozero() / TORUS_SCALE;
    double u2 = (double)sec_rand32_nozero() / TORUS_SCALE;
    
    // Box-Muller transform
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * acos(-1.0) * u2);
    
    // Scale to Torus range (2^32)
    return (uint32_t)(z0 * gaus_alpha * TORUS_SCALE);
}