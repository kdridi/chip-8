#pragma once

#include <stdint.h>
#include <stdbool.h>

void chip8_rom_load(uint8_t *data, uint16_t size);
void chip8_set_key_pressed(int8_t key_pressed);
void chip8_read_screen(void *data, void(*f)(void *data, uint_8 x, uint_8 y, bool on));
void chip8_reset(void);
void chip8_step(void);
