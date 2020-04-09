#include <stdint.h>
#include "mem.h"
using namespace std;

enum instr {
    BRK = 0x00,
    PHP = 0x08,
    BPL = 0x10,
    CLC = 0x18,
    ORA = 0x01,
    ASL = 0x06,
    JSR = 0x20,
    BIT = 0x24,
    PLP = 0x28,
    BMI = 0x30,
    SEC = 0x38,
    AND = 0x21,
    ROL = 0x26,
    RTI = 0x40,
    PHA = 0x48,
    JMP = 0x4c, JMP2 = 0x6c,
    BVC = 0x50,
    CLI = 0x58,
    EOR = 0x41,
    LSR = 0x46,
    RTS = 0x60,
    PLA = 0x68,
    BVS = 0x70,
    SEI = 0x78,
    ADC1 = 0x61, ADC2 = 0x65, ADC3 = 0x69, ADC4 = 0x6d,
    ADC5 = 0x71, ADC6 = 0x75, ADC7 = 0x79, ADC8 = 0x7d,
    ROR = 0x66,
    STY = 0x84,
    DEY = 0x88,
    BCC = 0x90,
    TYA = 0x98,
    STA = 0x81,
    STX = 0x86,
    TXA = 0x8a,
    TXS = 0x9a,
    LDY = 0xa0,
    TAY = 0xa8,
    BCS = 0xb0,
    CLV = 0xb8,
    LDA = 0xa1,
    LDX = 0xa2,
    TAX = 0xaa,
    TSX = 0xba,
    CPY = 0xc0,
    INY = 0xc8,
    BNE = 0xd0,
    CLD = 0xd8,
    CMP = 0xc1,
    DEC = 0xc6,
    DEX = 0xca,
    CPX = 0xe0,
    INX = 0xe8,
    BEQ = 0xf0,
    SED = 0xf8,
    SBC = 0xe1,
    INC = 0xe6,
    NOP = 0xea
};

class Cpu {
    public:
	Cpu();
	void execute();
	uint16_t read_operand(uint8_t opcode);
	uint16_t read_operand_indexed_indirect();
	uint16_t read_operand_zero_page();
	uint16_t read_operand_imm();
	uint16_t read_operand_absolute();
	uint16_t read_operand_indirect();
	uint16_t read_operand_relative();
	uint16_t read_operand_indirect_indexed();
	uint16_t read_operand_zero_page_indexed_x();
	uint16_t read_operand_absolute_indexed_x();
	uint16_t read_operand_absolute_indexed_y();
	void ADC_execute(uint16_t operand);
	void chivato(uint16_t operand);
    private:
	Mem mem;
	uint8_t regA, regX, regY, regP;
	uint16_t regCP, regSP;
};
