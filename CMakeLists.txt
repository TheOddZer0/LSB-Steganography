cmake_minimum_required(VERSION 3.12)
project(LSB-Steganography VERSION 0.0.1 LANGUAGES C)

add_executable(lsb-stego algo.c main.c)
install(TARGETS lsb-stego 
        RUNTIME 
            COMPONENT bin)

