#include "secure_sample.h"

uint32_t sec_rand32() {
    uint32_t r;
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) return 0;
    read(fd, &r, sizeof(r));
    close(fd);
    return r;
}

uint32_t sec_rand32_nozero(){
    uint32_t r = 0;
    while (r == 0){
        r = sec_rand32();
    }
}

uint8_t sec_rand8() {
    uint8_t r;
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) return 0;
    read(fd, &r, sizeof(r));
    close(fd);
    return r;
}

uint32_t sec_gaussian() {
    double u1 = (double)sec_rand32_nozero() / TORUS_SCALE;
    double u2 = (double)sec_rand32_nozero() / TORUS_SCALE;
    
    // Box-Muller transform
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * acos(-1.0) * u2);
    
    // Scale to Torus range (2^32)
    return (uint32_t)(z0 * gaus_alpha * TORUS_SCALE);
}