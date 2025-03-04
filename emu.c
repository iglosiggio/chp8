#ifndef _STATIC_EMU_C
#define _STATIC_EMU_C
#include <stdio.h>
#include <assert.h>

static inline
void op_cls(emu_t *emu) {
    for (int row = 0; row < DISPLAY_ROWS; row++)
    for (int col = 0; col < DISPLAY_COLS; col++)
        emu->display[row][col] = 0;
}

static inline
void op_ret(emu_t *emu) {
    emu->SP--;
    emu->pc = emu->stack[emu->SP];
}

static inline
void op_jp(emu_t *emu, uint16_t addr) {
    emu->pc = addr;
}

static inline
void op_call(emu_t *emu, uint16_t addr) {
    emu->stack[emu->SP] = emu->pc;
    emu->SP++;
    emu->pc = addr;
}

static inline
void op_se(emu_t *emu, uint8_t reg, uint8_t val) {
    if (emu->V[reg] == val)
        emu->pc += 2;
}

static inline
void op_sne(emu_t *emu, uint8_t reg, uint8_t val) {
    if (emu->V[reg] != val)
        emu->pc += 2;
}

static inline
void op_se_regs(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    if (emu->V[reg1] == emu->V[reg2])
        emu->pc += 2;
}

static inline
void op_ld(emu_t *emu, uint8_t reg, uint16_t val) {
    emu->V[reg] = val;
}

static inline
void op_add(emu_t *emu, uint8_t reg, uint16_t val) {
    emu->V[reg] += val;
}

static inline
void op_ld_regs(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    emu->V[reg1] = emu->V[reg2];
}

static inline
void op_or(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    emu->V[reg1] |= emu->V[reg2];
}

static inline
void op_and(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    emu->V[reg1] &= emu->V[reg2];
}

static inline
void op_xor(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    emu->V[reg1] ^= emu->V[reg2];
}

static inline
void op_add_regs(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    uint16_t res = emu->V[reg1] + emu->V[reg2];
    emu->V[0xF] = res > 255;
    emu->V[reg1] = res;
}

static inline
void op_sub(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    emu->V[0xF] = emu->V[reg1] > emu->V[reg2];
    emu->V[reg1] -= emu->V[reg2];
}

static inline
void op_shr(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    emu->V[0xF] = emu->V[reg1] & 0x0001;
    emu->V[reg1] >>= 1;
}

static inline
void op_subn(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    emu->V[0xF] = emu->V[reg2] > emu->V[reg1];
    emu->V[reg1] = emu->V[reg2] - emu->V[reg1];    
}

static inline
void op_shl(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    emu->V[0xF] = (emu->V[reg1] >> 7) & 0x1;
    emu->V[reg1] <<= 1;
}

static inline
void op_sne_regs(emu_t *emu, uint8_t reg1, uint8_t reg2) {
    if (emu->V[reg1] != emu->V[reg2])
      emu->pc += 2;
}

static inline
void op_ld_I(emu_t *emu, uint16_t addr) {
    emu->I = addr;
}

static inline
void op_jp_V0(emu_t *emu, uint16_t addr) {
    emu->I = addr + emu->V[0x0];
}

static inline
void op_rnd(emu_t *emu, uint8_t reg, uint16_t val) {
    emu->V[reg] = rand() & val;
}

static inline
void op_display(emu_t *emu, uint8_t x, uint8_t y, uint8_t n) {
    x = emu->V[x];
    y = emu->V[y];

    emu->V[0xF] = 0;
    /* Each line of a sprite is 8 pixels => it's 1 byte */
    for (int line = 0; line < n; line++) {
        for (int px = 7; px >= 0; px--) {
            uint8_t rx = (x + 7 - px) % DISPLAY_COLS;
            uint8_t ry = (y + line) % DISPLAY_ROWS;

            if (BIT_SET(emu->memory[emu->I+line], px)) {
                if (emu->display[ry][rx] == 1)
                    emu->V[0xF] = 1;
                emu->display[ry][rx] ^= 1;
            }
        }
    }
}

static inline
void op_skp(emu_t *emu, uint8_t reg) {
    printf("STUB op_skp(reg: %02X)\n", reg);
    assert(false);
}

static inline
void op_skpn(emu_t *emu, uint8_t reg) {
    printf("STUB op_skpn(reg: %02X)\n", reg);
    assert(false);
}

// WARNING not sure how the delay timer should behave
static inline
void op_ld_Vx_DT(emu_t *emu, uint8_t reg) {
    emu->V[reg] = emu->DT;
}

static inline
void op_ld_Vx_K(emu_t *emu, uint8_t reg) {
    printf("STUB op_ld_Vx_K(reg: %02X)\n", reg);
    assert(false);
}

static inline
void op_ld_DT_Vx(emu_t *emu, uint8_t reg) {
    emu->DT = emu->V[reg];
}

// WARNING not sure how the Sound Timer should behave
static inline
void op_ld_ST_Vx(emu_t *emu, uint8_t reg) {
    emu->ST = emu->V[reg];
}

static inline
void op_add_I(emu_t *emu, uint8_t reg) {
    emu->I += emu->V[reg];
}

static inline
void op_ld_F(emu_t *emu, uint8_t reg) {
    printf("STUB op_ld_F(reg: %02X)\n", reg);
    assert(false);
}

static inline
void op_ld_BCD(emu_t *emu, uint8_t reg) {
    printf("STUB op_ld_BCD(reg: %02X)\n", reg);
    assert(false);
}

static inline
void op_ld_array_Vx(emu_t *emu, uint8_t reg) {
    for (uint16_t i = 0; i <= reg; i++)
        emu->memory[emu->I + i] = emu->V[i];
}

static inline
void op_ld_Vx_array(emu_t *emu, uint8_t reg) {
    for (uint16_t i = 0; i <= reg; i++)
        emu->V[i] = emu->memory[emu->I + i];
}

static inline
void handle_instruction(emu_t *emu, uint16_t ins) {
    uint8_t reg1 = NIBBLE_2(ins);
    uint8_t reg2 = NIBBLE_1(ins);
    uint8_t val  = BYTE_0(ins);
    uint16_t addr = ins & 0x0FFF;

    switch (NIBBLE_3(ins)) {
        case 0x0:
            if (val == 0xE0)      op_cls(emu);
            else if (val == 0xEE) op_ret(emu);
            else goto unknown;
            break;
        case 0x1: op_jp      (emu, addr);       break;
        case 0x2: op_call    (emu, addr);       break;
        case 0x3: op_se      (emu, reg1, val);  break;
        case 0x4: op_sne     (emu, reg1, val);  break;
        case 0x5: op_se_regs (emu, reg1, reg2); break;
        case 0x6: op_ld      (emu, reg1, val);  break;
        case 0x7: op_add     (emu, reg1, val);  break;
        case 0x8:
            switch (NIBBLE_0(ins)) {
                case 0x0: op_ld_regs  (emu, reg1, reg2); break;
                case 0x1: op_or       (emu, reg1, reg2); break;
                case 0x2: op_and      (emu, reg1, reg2); break;
                case 0x3: op_xor      (emu, reg1, reg2); break;
                case 0x4: op_add_regs (emu, reg1, reg2); break;
                case 0x5: op_sub      (emu, reg1, reg2); break;
                case 0x6: op_shr      (emu, reg1, reg2); break;
                case 0x7: op_subn     (emu, reg1, reg2); break;
                case 0xE: op_shl      (emu, reg1, reg2); break;
                default:  goto unknown;
            }
            break;
        case 0x9: op_sne_regs (emu, reg1, reg2); break;
        case 0xA: op_ld_I     (emu, addr);       break;
        case 0xB: op_jp_V0    (emu, addr);       break;
        case 0xC: op_rnd      (emu, reg1, val);  break;
        case 0xD: op_display  (emu, reg1, reg2, NIBBLE_0(ins)); break;
        case 0xE:
            if (val == 0x9E)      op_skp(emu, reg1);
            else if (val == 0xA1) op_skpn(emu, reg1);
            else goto unknown;
            break;
        case 0xF:
            switch (val) {
                case 0x07: op_ld_Vx_DT    (emu, reg1); break;
                case 0x0A: op_ld_Vx_K     (emu, reg1); break;
                case 0x15: op_ld_DT_Vx    (emu, reg1); break;
                case 0x18: op_ld_ST_Vx    (emu, reg1); break;
                case 0x1E: op_add_I       (emu, reg1); break;
                case 0x29: op_ld_F        (emu, reg1); break;
                case 0x33: op_ld_BCD      (emu, reg1); break;
                case 0x55: op_ld_array_Vx (emu, reg1); break;
                case 0x65: op_ld_Vx_array (emu, reg1); break;
            }
            break;
        default:
        unknown:
            printf("INSTRUCTION %04X NOT FOUND\n", ins);
            break;
    }
}
#endif
