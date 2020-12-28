#include <stdint.h>
using namespace std;

const uint16_t NMI_INT_VECTOR_ADDR = 0xFFFA;
const uint16_t RST_INT_VECTOR_ADDR = 0xFFFC;
const uint16_t BRK_INT_VECTOR_ADDR = 0xFFFE;
const int RAM_SIZE = 0x10000;
const int PPU_RAM_SIZE = 0x4000;
const int PRG_ROM_SIZE = 0x4000;
const int PRG_ROM_START = 0x8000;
const int HEADER_SIZE = 16;
const int CHR_ROM_SIZE = 0x2000;
const int PPU_CHR_ROM_START = 0x0;
const int NAMETABLE_SIZE = 0x3C0;

enum addr_mode_t {
    IMMEDIATE,
    ZERO_PAGE,
    ZERO_PAGE_X,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    INDIRECT_X,
    INDIRECT_Y,
    RELATIVE,
    INDIRECT,
    IMPLIED
};

enum instr {
    BRK = 0x00,
    PHP = 0x08,
    BPL = 0x10,
    CLC = 0x18,
    ORA1 = 0x01, ORA2 = 0x05, ORA3 = 0x09, ORA4 = 0x0d,
    ORA5 = 0x11, ORA6 = 0x15, ORA7 = 0x19, ORA8 = 0x1d,
    ASL1 = 0x06, ASL2 = 0x0a, ASL3 = 0x0e, ASL4 = 0x16, ASL5 = 0x1e,
    JSR = 0x20,
    BIT1 = 0x24, BIT2 = 0x2c,
    PLP = 0x28,
    BMI = 0x30,
    SEC = 0x38,
    AND1 = 0x21, AND2 = 0x25, AND3 = 0x29, AND4 = 0x2d,
    AND5 = 0x31, AND6 = 0x35, AND7 = 0x39, AND8 = 0x3d,
    ROL1 = 0x26, ROL2 = 0x2a, ROL3 = 0x2e, ROL4 = 0x36, ROL5 = 0x3e,
    RTI = 0x40,
    PHA = 0x48,
    JMP1 = 0x4c, JMP2 = 0x6c,
    BVC = 0x50,
    CLI = 0x58,
    EOR1 = 0x41, EOR2 = 0x45, EOR3 = 0x49, EOR4 = 0x4d,
    EOR5 = 0x51, EOR6 = 0x55, EOR7 = 0x59, EOR8 = 0x5d,
    LSR1 = 0x46, LSR2 = 0x4a, LSR3 = 0x4e, LSR4 = 0x56, LSR5 = 0x5e,
    RTS = 0x60,
    PLA = 0x68,
    BVS = 0x70,
    SEI = 0x78,
    ADC1 = 0x61, ADC2 = 0x65, ADC3 = 0x69, ADC4 = 0x6d,
    ADC5 = 0x71, ADC6 = 0x75, ADC7 = 0x79, ADC8 = 0x7d,
    ROR1 = 0x66, ROR2 = 0x6a, ROR3 = 0x6e, ROR4 = 0x76, ROR5 = 0x7e,
    STY1 = 0x84, STY2 = 0x8c, STY3 = 0x94,
    DEY = 0x88,
    BCC = 0x90,
    TYA = 0x98,
    STA1 = 0x81, STA2 = 0x85, STA3 = 0x8d,
    STA4 = 0x91, STA5 = 0x95, STA6 = 0x99, STA7 = 0x9d,
    STX1 = 0x86, STX2 = 0x8e, STX3 = 0x96,
    TXA = 0x8a,
    TXS = 0x9a,
    LDY1 = 0xa0, LDY2 = 0xa4, LDY3 = 0xac, LDY4 = 0xb4, LDY5 = 0xbc,
    TAY = 0xa8,
    BCS = 0xb0,
    CLV = 0xb8,
    LDA1 = 0xa1, LDA2 = 0xa5, LDA3 = 0xa9, LDA4 = 0xad,
    LDA5 = 0xb1, LDA6 = 0xb5, LDA7 = 0xb9, LDA8 = 0xbd,
    LDX1 = 0xa2, LDX2 = 0xa6, LDX3 = 0xae, LDX4 = 0xb6, LDX5 = 0xbe,
    TAX = 0xaa,
    TSX = 0xba,
    CPY1 = 0xc0, CPY2 = 0xc4, CPY3 = 0xcc,
    INY = 0xc8,
    BNE = 0xd0,
    CLD = 0xd8,
    CMP1 = 0xc1, CMP2 = 0xc5, CMP3 = 0xc9, CMP4 = 0xcd,
    CMP5 = 0xd1, CMP6 = 0xd5, CMP7 = 0xd9, CMP8 = 0xdd,
    DEC1 = 0xc6, DEC2 = 0xce, DEC3 = 0xd6, DEC4 = 0xde,
    DEX = 0xca,
    CPX1 = 0xe0, CPX2 = 0xe4, CPX3 = 0xec,
    INX = 0xe8,
    BEQ = 0xf0,
    SED = 0xf8,
    SBC1 = 0xe1, SBC2 = 0xe5, SBC3 = 0xe9, SBC4 = 0xed,
    SBC5 = 0xf1, SBC6 = 0xf5, SBC7 = 0xf9, SBC8 = 0xfd,
    INC1 = 0xe6, INC2 = 0xee, INC3 = 0xf6, INC4 = 0xfe,
    NOP = 0xea
};
