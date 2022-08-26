cmake_minimum_required(VERSION 3.23)
project(chip_8 C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c2x")

add_executable(${PROJECT_NAME}
        main.c
        chip8.c
        chip8.h
        chip8_rom_load.c
        chip8_cpu_0.c
        chip8_cpu_1.c
        chip8_cpu_2.c
        chip8_cpu_3.c
        chip8_cpu_4.c
        chip8_cpu_5.c
        chip8_cpu_6.c
        chip8_cpu_7.c
        chip8_cpu_8.c
        chip8_cpu_9.c
        chip8_cpu_a.c
        chip8_cpu_b.c
        chip8_cpu_c.c
        chip8_cpu_d.c
        chip8_cpu_e.c
        chip8_cpu_f.c
        chip8_disassemble.c)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
