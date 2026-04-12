#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/random.h>
#include "params.h"

uint32_t sec_rand32();
uint32_t sec_rand32_nozero();
uint8_t sec_rand_bin();
uint32_t sec_gaussian();