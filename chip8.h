//
// Created by kdridi on 26/08/22.
//

#pragma once

#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#define RAM_SIZE (4 * 1024)
#define REGISTERS_COUNT 16
#define STACK_SIZE 16
#define FONTS_START_ADDRESS 0x50
#define FONT_BLOCK_SIZE 5
#define FONT_OFFSET(n) (FONTS_START_ADDRESS + ((n) * FONT_BLOCK_SIZE))
#define KEYS_COUNT 16

#define SCREEN_W 64
#define SCREEN_H 32
#define SCREEN_S 10

typedef union {
    uint16_t value;
    struct {
        uint8_t h : 8;
        uint8_t l : 8;
    };
    struct {
        union {
            uint8_t nn;
            struct {
                uint8_t n : 4;
                uint8_t y : 4;
            };
        };
        uint8_t x : 4;
        uint8_t f : 4;
    };
    struct {
        uint16_t nnn: 12;
        uint8_t _: 4;
    };
} opcode_t;

static_assert(sizeof(opcode_t) == 2, "opcode is not 2 bytes");

typedef void (*opcode_handler_t)(void);

typedef struct {
    struct {
        const char *name;
        uint16_t length;
    } rom_file;
    char ram[RAM_SIZE];
    uint8_t screen[SCREEN_W * SCREEN_H];
    bool screen_redraw;
    struct {
        opcode_t opcode;
        uint16_t pc;
        uint16_t i;
        uint8_t v[REGISTERS_COUNT];
        uint16_t stack[STACK_SIZE];
        uint16_t sp;
        uint8_t delay_timer;
        uint8_t sound_timer;
    } cpu;
    bool keys[KEYS_COUNT];
    int8_t key_pressed;
} chip8_t;

extern chip8_t chip8;

#define X chip8.cpu.opcode.x
#define Y chip8.cpu.opcode.y
#define N chip8.cpu.opcode.n
#define NN chip8.cpu.opcode.nn
#define NNN chip8.cpu.opcode.nnn
#define V(n) chip8.cpu.v[n]
#define VX V(X)
#define VY V(Y)
#define V0 V(0x0)
#define VF V(0xF)

#define RAM(addr) chip8.ram[(addr)]
#define SCREEN(x, y) chip8.screen[(x) + (y) * SCREEN_W]
#define KEY_IS_PRESSED(n) chip8.keys[n]
#define SKIP_NEXT_IF(cond) do { if (cond) { PC += 2; } } while (false)

#define STACK_PUSH(n) chip8.cpu.stack[chip8.cpu.sp++] = (n)
#define STACK_POP() chip8.cpu.stack[--chip8.cpu.sp]

#define CALL(addr) do { STACK_PUSH(PC); PC = (addr); } while (false)

#define I chip8.cpu.i
#define PC chip8.cpu.pc
#define DT chip8.cpu.delay_timer
#define ST chip8.cpu.sound_timer

void chip8_init(void);
void chip8_reset(void);
int chip8_rom_load(const char *filename);

void chip8_disassemble(void);
void chip8_fetch();
void chip8_execute();

void chip8_cpu_0___(void);
void chip8_cpu_1NNN(void);
void chip8_cpu_2NNN(void);
void chip8_cpu_3XNN(void);
void chip8_cpu_4XNN(void);
void chip8_cpu_5XY0(void);
void chip8_cpu_6XNN(void);
void chip8_cpu_7XNN(void);
void chip8_cpu_8___(void);
void chip8_cpu_9XY0(void);
void chip8_cpu_ANNN(void);
void chip8_cpu_BNNN(void);
void chip8_cpu_CXNN(void);
void chip8_cpu_DXYN(void);
void chip8_cpu_E___(void);
void chip8_cpu_F___(void);
