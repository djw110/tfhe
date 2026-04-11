cmake_minimum_required(VERSION 3.31)
project(TFHE C)


add_executable(tfhe_mul 
    src/main.c
    src/tlwe.c
    src/trlwe.c
    src/params.c
    src/secure_sample.c
)
target_include_directories(tfhe_mul PRIVATE src)
target_link_libraries(tfhe_mul PRIVATE m)