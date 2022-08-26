//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

static void chip8_cpu_draw_sprite(uint8_t x, uint8_t y, uint8_t height);

// Draw a sprite at coordinate (VX, VY) with N bytes of sprite data starting at the address stored in register I
void chip8_cpu_DXYN() {
    // printf("DRW V%X, V%X, $%02X", chip8.cpu.opcode.x, chip8.cpu.opcode.y, chip8.cpu.opcode.n);
    // fflush(stdout);
    chip8_cpu_draw_sprite(chip8.cpu.v[chip8.cpu.opcode.x], chip8.cpu.v[chip8.cpu.opcode.y], chip8.cpu.opcode.n);
    chip8.screen_redraw = true;
}

static void chip8_cpu_draw_sprite(uint8_t x, uint8_t y, uint8_t height) {
    uint8_t pixel;
    chip8.cpu.v[0xF] = 0;
    for (int yline = 0; yline < height; yline++) {
        pixel = chip8.ram[chip8.cpu.i + yline];
        for (int xline = 0; xline < 8; xline++) {
            if ((pixel & (0x80 >> xline)) != 0) {
                if (chip8.screen[(x + xline + ((y + yline) * SCREEN_W))] == 1) {
                    chip8.cpu.v[0xF] = 1;
                }
                chip8.screen[x + xline + ((y + yline) * SCREEN_W)] ^= 1;
            }
        }
    }
}
