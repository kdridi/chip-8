//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

static opcode_handler_t handlers[0x10];

void chip8_cpu_8___() {
    opcode_handler_t h = handlers[chip8.cpu.opcode.n];
    if (h) {
        h();
    } else {
        fprintf(stderr, "Unknown opcode: 0x%04x\n", chip8.cpu.opcode);
        fflush(stdout);
        abort();
    }
}

// Set register VX to the value of register VY
static void chip8_cpu_8XY0() {
    VX = VY;
}

// Set register VX to the value of register VX or VY
static void chip8_cpu_8XY1() {
    VX |= VY;
}

// Set register VX to the value of register VX and VY
static void chip8_cpu_8XY2() {
    VX &= VY;
}

// Set register VX to the value of register VX xor VY
static void chip8_cpu_8XY3() {
    VX ^= VY;
}

// Add the value of register VY to register VX. VF is set to 1 when there's a carry, and to 0 when there isn't
static void chip8_cpu_8XY4() {
    uint16_t sum = VX + VY;
    VF = sum > 0x0ff ? 1 : 0;
    VX = sum & 0x0ff;
}

// Subtract the value of register VY from register VX. VF is set to 0 when there's a borrow, and to 1 when there isn't
static void chip8_cpu_8XY5() {
    int16_t diff = VX - VY;
    VF = diff < 0x00 ? 1 : 0;
    VX = diff & 0x0ff;
}

// Store the value of register VY shifted right by one in register VX. VF is set to the least significant bit of VY before the shift. VY is unchanged.
static void chip8_cpu_8XY6() {
    VF = VY & 0x1;
    VX >>= 1;
}

// Set register VX to the value of register VY minus register VX. VF is set to 0 when there's a borrow, and to 1 when there isn't
static void chip8_cpu_8XY7() {
    int16_t diff = VY - VX;
    VF = diff < 0 ? 1 : 0;
    VX = diff & 0x0ff;
}

// Store the value of register VY shifted left by one in register VX. VF is set to the most significant bit of VY before the shift. VY is unchanged.
static void chip8_cpu_8XYE() {
    VF = VY >> 7;
    VX <<= 1;
}

__attribute__((constructor))
static void chip8_cpu_8___init() {
    memset(handlers, 0, sizeof(handlers));
    handlers[0x0] = chip8_cpu_8XY0;
    handlers[0x1] = chip8_cpu_8XY1;
    handlers[0x2] = chip8_cpu_8XY2;
    handlers[0x3] = chip8_cpu_8XY3;
    handlers[0x4] = chip8_cpu_8XY4;
    handlers[0x5] = chip8_cpu_8XY5;
    handlers[0x6] = chip8_cpu_8XY6;
    handlers[0x7] = chip8_cpu_8XY7;
    handlers[0xE] = chip8_cpu_8XYE;
}
