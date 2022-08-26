//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

static opcode_handler_t handlers[0x100];

void chip8_cpu_F___() {
    opcode_handler_t h = handlers[chip8.cpu.opcode.nn];
    if (h) {
        h();
    } else {
        fprintf(stderr, "Unknown opcode: 0x%04x\n", chip8.cpu.opcode);
        fflush(stdout);
        abort();
    }
}

// Store the current value of the delay timer in register VX
static void chip8_cpu_FX07() {
    // printf("LD V%X, DT", chip8.cpu.opcode.x);
    // fflush(stdout);
    chip8.cpu.v[chip8.cpu.opcode.x] = chip8.cpu.delay_timer;
}

// Wait for a key press, store the hex value of the key in register VX
static void chip8_cpu_FX0A() {
    // printf("LD V%X, K", chip8.cpu.opcode.x);
    // fflush(stdout);
    if (chip8.key_pressed >= 0) {
        chip8.cpu.v[chip8.cpu.opcode.x] = chip8.key_pressed;
        chip8.key_pressed = -1;
    } else {
        chip8.cpu.pc -= 2;
    }
}

// Set the delay timer to the value of register VX
static void chip8_cpu_FX15() {
    // printf("LD DT, V%X", chip8.cpu.opcode.x);
    // fflush(stdout);
    chip8.cpu.delay_timer = chip8.cpu.v[chip8.cpu.opcode.x];
}

// Set the sound timer to the value of register VX
static void chip8_cpu_FX18() {
    // printf("LD ST, V%X", chip8.cpu.opcode.x);
    // fflush(stdout);
    chip8.cpu.sound_timer = chip8.cpu.v[chip8.cpu.opcode.x];
}

// Add the value of register VX to register I
static void chip8_cpu_FX1E() {
    // printf("ADD I, V%X", chip8.cpu.opcode.x);
    // fflush(stdout);
    int32_t sum = chip8.cpu.i + chip8.cpu.v[chip8.cpu.opcode.x];
    if (sum > 0xfff) {
        chip8.cpu.v[0xf] = 1;
    } else {
        chip8.cpu.v[0xf] = 0;
    }
    chip8.cpu.i = sum & 0xfff;
}

// Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
static void chip8_cpu_FX29() {
    // printf("LD F, V%X", chip8.cpu.opcode.x);
    // fflush(stdout);
    chip8.cpu.i = FONT_OFFSET(chip8.cpu.v[chip8.cpu.opcode.x] & 0x0f);
}

// Store the binary-coded decimal equivalent of the value stored in register VX at address I, I+1, and I+2
static void chip8_cpu_FX33() {
    // printf("LD B, V%X", chip8.cpu.opcode.x);
    // fflush(stdout);
    chip8.ram[chip8.cpu.i] = chip8.cpu.v[chip8.cpu.opcode.x] / 100;
    chip8.ram[chip8.cpu.i + 1] = (chip8.cpu.v[chip8.cpu.opcode.x] / 10) % 10;
    chip8.ram[chip8.cpu.i + 2] = chip8.cpu.v[chip8.cpu.opcode.x] % 10;
}

// Store the values of registers V0 to VX inclusive in memory starting at address I. I is set to I + X + 1 after operation²
static void chip8_cpu_FX55() {
    // printf("LD [I], V%X", chip8.cpu.opcode.x);
    // fflush(stdout);
    for (int i = 0; i <= chip8.cpu.opcode.x; ++i)
        chip8.ram[chip8.cpu.i++] = chip8.cpu.v[i];
}

// Fill registers V0 to VX inclusive with the values stored in memory starting at address I. I is set to I + X + 1 after operation
static void chip8_cpu_FX65() {
    // printf("LD V%X, [I]", chip8.cpu.opcode.x);
    // fflush(stdout);
    for (int i = 0; i <= chip8.cpu.opcode.x; ++i)
        chip8.cpu.v[i] = chip8.ram[chip8.cpu.i++];
}

__attribute__((constructor))
static void chip8_cpu_F___init() {
    memset(handlers, 0, sizeof(handlers));
    handlers[0x07] = chip8_cpu_FX07;
    handlers[0x0a] = chip8_cpu_FX0A;
    handlers[0x15] = chip8_cpu_FX15;
    handlers[0x18] = chip8_cpu_FX18;
    handlers[0x1e] = chip8_cpu_FX1E;
    handlers[0x29] = chip8_cpu_FX29;
    handlers[0x33] = chip8_cpu_FX33;
    handlers[0x55] = chip8_cpu_FX55;
    handlers[0x65] = chip8_cpu_FX65;
}
