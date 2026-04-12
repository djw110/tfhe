#define TORUS_BITS 32
#define TORUS_SCALE (1ULL << TORUS_BITS)
#define TORUS_STEP  (TORUS_SCALE >> 3)  // 1/8

#define TORUS_0     0U
#define TORUS_0_125   (1U * TORUS_STEP)
#define TORUS_0_25   (2U * TORUS_STEP)
#define TORUS_0_375   (3U * TORUS_STEP)
#define TORUS_0_5   (4U * TORUS_STEP)
#define TORUS_0_625   (5U * TORUS_STEP)
#define TORUS_0_75   (6U * TORUS_STEP)
#define TORUS_0_875   (7U * TORUS_STEP)

extern const int n; // ring dimension
extern const int l; // decomposition levels
extern const int base_log; //bits per level (Base = 2^base_log)
extern const double gaus_alpha; // noise parameter