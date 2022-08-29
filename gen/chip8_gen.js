const lines = [];
lines.push(`#include <memory.h>`);
lines.push(`#include <stdlib.h>`);
lines.push(`#include <stdint.h>`);
lines.push(`#include <stdio.h>`);
lines.push(`#include <stdbool.h>`);
lines.push(``);
lines.push(`static uint16_t pc = 0x200;`);
lines.push(`static uint16_t i = 0;`);
lines.push(`static uint16_t sp = 0;`);
lines.push(`static uint16_t stack[16];`);
lines.push(`static uint8_t memory[4096];`);
lines.push(`static uint8_t v[16];`);
lines.push(`static bool vram[64 * 32];`);
lines.push(`static bool dirty = true;`);
lines.push(`static int8_t key_pressed = -1;`);
lines.push(`static uint8_t delay_timer = 0;`);
lines.push(`static uint8_t sound_timer = 0;`);
lines.push(`static uint8_t fonts[] = {`);
lines.push(`    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0`);
lines.push(`    0x20, 0x60, 0x20, 0x20, 0x70, // 1`);
lines.push(`    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2`);
lines.push(`    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3`);
lines.push(`    0x90, 0x90, 0xF0, 0x10, 0x10, // 4`);
lines.push(`    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5`);
lines.push(`    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6`);
lines.push(`    0xF0, 0x10, 0x20, 0x40, 0x40, // 7`);
lines.push(`    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8`);
lines.push(`    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9`);
lines.push(`    0xF0, 0x90, 0xF0, 0x90, 0x90, // A`);
lines.push(`    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B`);
lines.push(`    0xF0, 0x80, 0x80, 0x80, 0xF0, // C`);
lines.push(`    0xE0, 0x90, 0x90, 0x90, 0xE0, // D`);
lines.push(`    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E`);
lines.push(`    0xF0, 0x80, 0xF0, 0x80, 0x80  // F`);
lines.push(`};`);
lines.push(`static uint8_t rom[4096];`);
lines.push(`static uint16_t rom_size = 0;`);
lines.push(``);

lines.push(`extern void chip8_rom_load(uint8_t *data, uint16_t size) {`);
lines.push(`    memcpy(rom, data, size);`);
lines.push(`    rom_size = size;`);
lines.push(`}`);
lines.push(``);

lines.push(`extern void chip8_set_key_pressed(int8_t kp) {`);
lines.push(`    key_pressed = kp;`);
lines.push(`}`);
lines.push(``);

lines.push(`extern void chip8_read_screen(void *data, void(*f)(void *data, uint_8 x, uint_8 y, bool on)) {`);
lines.push(`    for (uint_8 y = 0, n = 0; y < 32; y++)`);
lines.push(`        for (uint_8 x = 0; x < 64; x++, n++)`);
lines.push(`            f(data, x, y, vram[n]);`);
lines.push(`}`);
lines.push(``);

lines.push(`extern void chip8_reset(void) {`);
lines.push(`    pc = 0x200;`);
lines.push(`    i = 0;`);
lines.push(`    sp = 0;`);
lines.push(`    memset(stack, 0, sizeof(stack));`);
lines.push(`    memset(memory, 0, sizeof(memory));`);
lines.push(`    memset(v, 0, sizeof(v));`);
lines.push(`    memset(vram, 0, sizeof(vram));`);
lines.push(`    dirty = true;`);
lines.push(`    key_pressed = -1;`);
lines.push(`    delay_timer = 0;`);
lines.push(`    sound_timer = 0;`);
lines.push(`    memcpy(memory, fonts, sizeof(fonts));`);
lines.push(`    memcpy(memory + 0x200, rom, rom_size);`);
lines.push(`}`);
lines.push(``);

new Array(0x1000).fill(0).forEach(function(_, i) {
    lines.push(`static void op_0${i.toString(16).padStart(3, '0')}(void) {`);
    switch (i) {
        case 0x00E0:
            lines.push(`    // CLS`);
            lines.push(`    memset(vram, 0, sizeof(vram));`);
            lines.push(`    dirty = true;`);
            break;
        case 0x00EE:
            lines.push(`    // RET`);
            lines.push(`    pc = stack[--sp];`);
            break;
        default:
            lines.push(`    // SYS ${i.toString(16).padStart(3, '0')}`);
            lines.push(`    abort();`);
            break;
    }
    lines.push(`}`);
    lines.push(``);
})

new Array(0x1000).fill(0).forEach(function(_, i) {
    lines.push(`static void op_1${i.toString(16).padStart(3, '0')}(void) {`);
    lines.push(`    // JP $${i.toString(16).padStart(4, '0')}`);
    lines.push(`    pc = 0x${i.toString(16).padStart(4, '0')};`);
    lines.push(`}`);
    lines.push(``);
})

new Array(0x1000).fill(0).forEach(function(_, i) {
    lines.push(`static void op_2${i.toString(16).padStart(3, '0')}(void) {`);
    lines.push(`    // CALL $${i.toString(16).padStart(4, '0')}`);
    lines.push(`    stack[sp++] = pc;`);
    lines.push(`    pc = 0x${i.toString(16).padStart(4, '0')};`);
    lines.push(`}`);
    lines.push(``);
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_3${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // SE V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    if (v[0x${x.toString(16).padStart(1, '0')}] == 0x${nn.toString(16).padStart(3, '0')}) {`);
        lines.push(`        pc += 2;`);
        lines.push(`    }`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_4${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // SNE V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    if (v[0x${x.toString(16).padStart(1, '0')}] != 0x${nn.toString(16).padStart(3, '0')}) {`);
        lines.push(`        pc += 2;`);
        lines.push(`    }`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_5${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // SE V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    if (v[0x${x.toString(16).padStart(1, '0')}] == 0x${nn.toString(16).padStart(3, '0')}) {`);
        lines.push(`        pc += 2;`);
        lines.push(`    }`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_6${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // LD V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = 0x${nn.toString(16).padStart(3, '0')};`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_7${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // ADD V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] += 0x${nn.toString(16).padStart(3, '0')};`);
        lines.push(`}`);
        lines.push(``);
    })
})


new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x10).fill(0).forEach(function(_, y) {
        new Array(0x10).fill(0).forEach(function(_, n) {
            lines.push(`static void op_8${x.toString(16).padStart(1, '0')}${y.toString(16).padStart(1, '0')}${n.toString(16).padStart(1, '0')}(void) {`);
            switch (n) {
                case 0x0:
                    lines.push(`    // LD V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = v[0x${y.toString(16).padStart(1, '0')}];`);
                    break;
                case 0x1:
                    lines.push(`    // OR V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] |= v[0x${y.toString(16).padStart(1, '0')}];`);
                    break;
                case 0x2:
                    lines.push(`    // AND V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] &= v[0x${y.toString(16).padStart(1, '0')}];`);
                    break;
                case 0x3:
                    lines.push(`    // XOR V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] ^= v[0x${y.toString(16).padStart(1, '0')}];`);
                    break;
                case 0x4:
                    lines.push(`    // ADDC V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    int16_t result = v[0x${x.toString(16).padStart(1, '0')}] + v[0x${y.toString(16).padStart(1, '0')}];`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = result & 0x0ff;`);
                    lines.push(`    v[0xf] = result > 0x0ff ? 1 : 0;`);
                    break;
                case 0x5:
                    lines.push(`    // SUBC V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    int16_t result = v[0x${x.toString(16).padStart(1, '0')}] - v[0x${y.toString(16).padStart(1, '0')}];`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = result & 0x0ff;`);
                    lines.push(`    v[0xf] = result < 0x000 ? 0 : 1;`);
                    break;
                case 0x6:
                    lines.push(`    // SHR V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = v[0x${y.toString(16).padStart(1, '0')}] >> 1;`);
                    lines.push(`    v[0xf] = v[0x${y.toString(16).padStart(1, '0')}] & 0x01;`);
                    break;
                case 0x7:
                    lines.push(`    // SUBNC V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    int16_t result = v[0x${y.toString(16).padStart(1, '0')}] - v[0x${x.toString(16).padStart(1, '0')}];`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = result & 0x0ff;`);
                    lines.push(`    v[0xf] = result < 0x000 ? 0 : 1;`);
                    break;
                case 0xe:
                    lines.push(`    // SHL V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}`);
                    lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = v[0x${y.toString(16).padStart(1, '0')}] << 1;`);
                    lines.push(`    v[0xf] = v[0x${y.toString(16).padStart(1, '0')}] & 0x80 ? 1 : 0;`);
                    break;
                default:
                    lines.push(`    // UNKNOWN`);
                    lines.push(`    fprintf(stderr, "Unknown opcode: 0x8${x.toString(16).padStart(1, '0')}${y.toString(16).padStart(1, '0')}${n.toString(16).padStart(1, '0')}\\n");`);
                    lines.push(`    abort();`);
                    break;
            }
            lines.push(`}`);
            lines.push(``);
        })
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_9${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // SE V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    if (v[0x${x.toString(16).padStart(1, '0')}] != 0x${nn.toString(16).padStart(3, '0')}) {`);
        lines.push(`        pc += 2;`);
        lines.push(`    }`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x1000).fill(0).forEach(function(_, nnn) {
    lines.push(`static void op_a${nnn.toString(16).padStart(3, '0')}(void) {`);
    lines.push(`    // LD I, $${nnn.toString(16).padStart(3, '0')}`);
    lines.push(`    i = 0x${nnn.toString(16).padStart(3, '0')};`);
    lines.push(`}`);
    lines.push(``);
})

new Array(0x1000).fill(0).forEach(function(_, nnn) {
    lines.push(`static void op_b${nnn.toString(16).padStart(3, '0')}(void) {`);
    lines.push(`    // JP V0, $${nnn.toString(16).padStart(3, '0')}`);
    lines.push(`    pc = 0x${nnn.toString(16).padStart(4, '0')} + v[0x0];`);
    lines.push(`}`);
    lines.push(``);
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_c${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        lines.push(`    // RND V${x.toString(16).toUpperCase().padStart(1, '0')}, $${nn.toString(16).padStart(2, '0')}`);
        lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = (rand() % 0x100) & 0x${nn.toString(16).padStart(3, '0')};`);
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x10).fill(0).forEach(function(_, y) {
        new Array(0x10).fill(0).forEach(function(_, n) {
            lines.push(`static void op_d${x.toString(16).padStart(1, '0')}${y.toString(16).padStart(1, '0')}${n.toString(16).padStart(1, '0')}(void) {`);
            lines.push(`    // DRW V${x.toString(16).toUpperCase().padStart(1, '0')}, V${y.toString(16).toUpperCase().padStart(1, '0')}, $${n.toString(16).padStart(1, '0')}`);
            lines.push(`    uint8_t pixel;`);
            lines.push(`    v[0xf] = 0;`);
            lines.push(`    for (int y = 0; y < ${n}; y++) {`);
            lines.push(`        pixel = memory[i + y];`);
            lines.push(`        for (int x = 0; x < 8; x++) {`);
            lines.push(`            if ((pixel & (0x80 >> x)) != 0) {`);
            lines.push(`                v[0xf] |= vram[(v[0x${y.toString(16).padStart(1, '0')}] + y) * 64 + v[0x${x.toString(16).padStart(1, '0')}] + x];`);
            lines.push(`                vram[(v[0x${y.toString(16).padStart(1, '0')}] + y) * 64 + v[0x${x.toString(16).padStart(1, '0')}] + x] ^= 1;`);
            lines.push(`            }`);
            lines.push(`        }`);
            lines.push(`    }`);
            lines.push(`    dirty = true;`);        
            lines.push(`}`);
            lines.push(``);
        })
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_e${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        switch (nn) {
            case 0x09e:
                lines.push(`    // SKP V${x.toString(16).toUpperCase().padStart(1, '0')}`);
                lines.push(`    if (key_pressed == v[0x${x.toString(16).padStart(1, '0')}]) {`);
                lines.push(`        pc += 2;`);
                lines.push(`    }`);
                break;
            case 0x0a1:
                lines.push(`    // SKNP V${x.toString(16).toUpperCase().padStart(1, '0')}`);
                lines.push(`    if (key_pressed != v[0x${x.toString(16).padStart(1, '0')}]) {`);
                lines.push(`        pc += 2;`);
                lines.push(`    }`);
                break;
            default:
                lines.push(`    // UNKNOWN`);
                lines.push(`    fprintf(stderr, "Unknown opcode: 0xe${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}\\n");`);
                lines.push(`    abort();`);
                break;
        }
        lines.push(`}`);
        lines.push(``);
    })
})

new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x100).fill(0).forEach(function(_, nn) {
        lines.push(`static void op_f${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}(void) {`);
        switch (nn) {
            case 0x07:
                lines.push(`    // LD V${x.toString(16).toUpperCase().padStart(1, '0')}, DT`);
                lines.push(`    v[0x${x.toString(16).padStart(1, '0')}] = delay_timer;`);
                break;
            case 0x0a:
                lines.push(`    // LD V${x.toString(16).toUpperCase().padStart(1, '0')}, KP`);
                lines.push(`    if (key_pressed >= 0) {`);
                lines.push(`        v[0x${x.toString(16).padStart(1, '0')}] = key_pressed;`);
                lines.push(`        key_pressed = -1;`);
                lines.push(`    } else {`);
                lines.push(`        pc -= 2;`);
                lines.push(`    }`);
                break;
            case 0x15:
                lines.push(`    // LD DT, V${x.toString(16).toUpperCase().padStart(1, '0')}`);
                lines.push(`    delay_timer = v[0x${x.toString(16).padStart(1, '0')}];`);
                break;
            case 0x18:
                lines.push(`    // LD ST, V${x.toString(16).toUpperCase().padStart(1, '0')}`);
                lines.push(`    sound_timer = v[0x${x.toString(16).padStart(1, '0')}];`);
                break;
            case 0x1e:
                lines.push(`    // ADD I, V${x.toString(16).toUpperCase().padStart(1, '0')}`);
                lines.push(`    int32_t sum = i + v[0x${x.toString(16).padStart(1, '0')}];`);
                lines.push(`    v[0xf] = (sum > 0x0fff) ? 1 : 0;`);
                lines.push(`    i = sum & 0x0fff;`);
                break;
            case 0x29:
                lines.push(`    // LD F, V${x.toString(16).toUpperCase().padStart(1, '0')}`);
                lines.push(`    i = v[0x${x.toString(16).padStart(1, '0')}] * 5;`);
                break;
            case 0x33:
                lines.push(`    // LD B, V${x.toString(16).toUpperCase().padStart(1, '0')}`);
                lines.push(`    memory[i] = v[0x${x.toString(16).padStart(1, '0')}] / 100;`);
                lines.push(`    memory[i + 1] = (v[0x${x.toString(16).padStart(1, '0')}] / 10) % 10;`);
                lines.push(`    memory[i + 2] = v[0x${x.toString(16).padStart(1, '0')}] % 10;`);
                break;
            case 0x55:
                lines.push(`    // LD [I], V${x.toString(16).toUpperCase().padStart(1, '0')}`);
                lines.push(`    for (int32_t n = 0; n <= 0x${x.toString(16).padStart(1, '0')}; n++) {`);
                lines.push(`        memory[i++] = v[n];`);
                lines.push(`    }`);
                break;
            case 0x65:
                lines.push(`    // LD V${x.toString(16).toUpperCase().padStart(1, '0')}, [I]`);
                lines.push(`    for (int32_t n = 0; n <= 0x${x.toString(16).padStart(1, '0')}; n++) {`);
                lines.push(`        v[n] = memory[i++];`);
                lines.push(`    }`);
                break;
            default:
                lines.push(`    // UNKNOWN`);
                lines.push(`    fprintf(stderr, "Unknown opcode: 0xf${x.toString(16).padStart(1, '0')}${nn.toString(16).padStart(2, '0')}\\n");`);
                lines.push(`    abort();`);
                break;
        }
        lines.push(`}`);
        lines.push(``);
    })
})

lines.push(`typedef void (*opcode_fn)(void);`);
lines.push(`opcode_fn opcode_fns[] = {`);
new Array(0x10).fill(0).forEach(function(_, x) {
    new Array(0x1000).fill(0).forEach(function(_, nnn) {
        lines.push(`    op_${x.toString(16).padStart(1, '0')}${nnn.toString(16).padStart(3, '0')},`);
    })
})
lines.push(`};`);
lines.push(``);

lines.push(`extern void chip8_step(void) {`);
lines.push(`    // LOG("pc: 0x%04x", pc);`);
lines.push(`    uint16_t opcode = memory[pc++] << 8;`);
lines.push(`    opcode |= memory[pc++];`);
lines.push(`    // LOG(", opcode: 0x%04x", opcode);`);
lines.push(`    opcode_fns[opcode]();`);
lines.push(`    // LOG("\\n");`);
lines.push(`}`);
lines.push(``);

console.log(lines.join('\n'));
