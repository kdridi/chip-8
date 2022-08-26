//
// Created by kdridi on 26/08/22.
//

#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>

static void chip8_cpu_draw_sprite(uint8_t x, uint8_t y, uint8_t height);

// Draw a sprite at coordinate (VX, VY) with N bytes of sprite data starting at the address stored in register I
void chip8_cpu_DXYN() {
    uint8_t pixel;
    VF = 0;
    for (int yline = 0; yline < N; yline++) {
        pixel = RAM(I + yline);
        for (int xline = 0; xline < 8; xline++) {
            if ((pixel & (0x80 >> xline)) != 0) {
                if (SCREEN(VX + xline, VY + yline) == 1) {
                    VF = 1;
                }
                SCREEN(VX + xline, VY + yline) ^= 1;
            }
        }
    }
    chip8.screen_redraw = true;
}
