#include <stdint.h>
#include <iostream>
#include "cpu.h"
using namespace std;

Cpu::Cpu() {
    regX = regY = regA = 0x00;
    regPC = mem.read_word(0xFFFC);
    regSP = 0x01ff;
    regP.C = regP.Z = regP.I = regP.D = regP.B = regP.V = regP.N = 0;
}

void Cpu::execute() {
    addr_mode_t addr_mode;
    uint8_t opcode = mem.read_byte(regPC);
    uint32_t addrop = read_operand(opcode, &addr_mode);
    uint16_t address = addrop >> 16;
    uint8_t operand = addrop & 0xFF;

    switch (opcode) {
	case ADC1: case ADC2: case ADC3: case ADC4:
	case ADC5: case ADC6: case ADC7: case ADC8:
	    ADC_execute(operand);
	    break;
	case AND1: case AND2: case AND3: case AND4:
	case AND5: case AND6: case AND7: case AND8:
	    AND_execute(operand);
	    break;
	case ASL2:
	    ASL_A_execute();
	    break;
	case ASL1: case ASL3: case ASL4: case ASL5:
	    ASL_mem_execute(address, operand);
	    break;
	case BCC:
	    BCC_execute(address);
	    break;
	case BCS:
	    BCS_execute(address);
	    break;
	case BEQ:
	    BEQ_execute(address);
	    break;
	case BIT1: case BIT2:
	    BIT_execute(operand);
	    break;
	case BMI:
	    BMI_execute(address);
	    break;
	case BNE:
	    BNE_execute(address);
	    break;
	case BPL:
	    BPL_execute(address);
	    break;
	case BRK:
	    BRK_execute();
	    break;
	case BVC:
	    BVC_execute(address);
	    break;
	case BVS:
	    BVS_execute(address);
	    break;
	case CLC:
	    CLC_execute();
	    break;
	case CLD:
	    CLD_execute();
	    break;
	case CLI:
	    CLI_execute();
	    break;
	case CLV:
	    CLV_execute();
	    break;
	case CMP1: case CMP2: case CMP3: case CMP4:
	case CMP5: case CMP6: case CMP7: case CMP8:
	    CMP_execute(operand);
	    break;
	case CPX1: case CPX2: case CPX3:
	    CPX_execute(operand);
	    break;
	case CPY1: case CPY2: case CPY3:
	    CPY_execute(operand);
	    break;
	case DEC1: case DEC2: case DEC3: case DEC4:
	    DEC_execute(address, operand);
	    break;
	case DEX:
	    DEX_execute();
	    break;
	case DEY:
	    DEY_execute();
	    break;
	case EOR1: case EOR2: case EOR3: case EOR4: 
	case EOR5: case EOR6: case EOR7: case EOR8: 
	    EOR_execute(operand);
	    break;
	case INC1: case INC2: case INC3: case INC4:
	    INC_execute(address, operand);
	    break;
	case INX:
	    INX_execute();
	    break;
	case INY:
	    INY_execute();
	    break;
	case JMP1: case JMP2:
	    JMP_execute(address);
	    break;
	case JSR:
	    JSR_execute(address);
	    break;
	case LDA1: case LDA2: case LDA3: case LDA4:
	case LDA5: case LDA6: case LDA7: case LDA8:
	    LDA_execute(operand);
	    break;
	case LDX1: case LDX2: case LDX3: case LDX4: case LDX5:
	    LDX_execute(operand);
	    break;
	case LDY1: case LDY2: case LDY3: case LDY4: case LDY5:
	    LDY_execute(operand);
	    break;
	case LSR1: case LSR3: case LSR4: case LSR5:
	    LSR_mem_execute(address, operand);
	    break;
	case LSR2:
	    LSR_A_execute();
	    break;
	case ORA1: case ORA2: case ORA3: case ORA4:
	case ORA5: case ORA6: case ORA7: case ORA8:
	    ORA_execute(operand);
	    break;
	case PHA:
	    PHA_execute();
	    break;
	case PHP:
	    PHP_execute();
	    break;
	case PLA:
	    PLA_execute();
	    break;
	case PLP:
	    PLP_execute();
	case ROL1: case ROL3: case ROL4: case ROL5:
	    ROL_mem_execute(address, operand);
	    break;
	case ROL2:
	    ROL_A_execute();
	    break;
	case ROR1: case ROR3: case ROR4: case ROR5:
	    ROR_mem_execute(address, operand);
	    break;
	case ROR2:
	    ROR_A_execute();
	    break;
	case RTI:
	    RTI_execute();
	    break;
	case RTS:
	    RTS_execute();
	    break;
	case SBC1: case SBC2: case SBC3: case SBC4:
	case SBC5: case SBC6: case SBC7: case SBC8:
	    SBC_execute(operand);
	    break;
	case SEC:
	    SEC_execute();
	    break;
	case SED:
	    SED_execute();
	    break;
	case SEI:
	    SEI_execute();
	    break;
	case STA1: case STA2: case STA3:
	case STA4: case STA5: case STA6: case STA7:
	    STA_execute(address);
	    break;
	case STX1: case STX2: case STX3:
	    STX_execute(address);
	    break;
	case STY1: case STY2: case STY3:
	    STY_execute(address);
	    break;
	case TAX:
	    TAX_execute();
	    break;
	case TAY:
	    TAY_execute();
	    break;
	case TSX:
	    TSX_execute();
	    break;
	case TXA:
	    TXA_execute();
	    break;
	case TXS:
	    TXS_execute();
	    break;
	case TYA:
	    TYA_execute();
	    break;
	default : 
	    NOP_execute();
    }
    add_clock_cycles(opcode, address, addr_mode);
    //debug_dump(opcode);
}

void Cpu::add_clock_cycles(uint8_t opcode, uint16_t address, addr_mode_t addr_mode) {
    //Add cycles depending on address mode and instruction
    switch (addr_mode) {
	case IMMEDIATE:
	    clock->cycles += 2;
	    break;
	case ZERO_PAGE:
	    clock->cycles += 3;
	    if (opcode == ASL1 ||
		opcode == ASL2 ||
		opcode == ASL3 ||
		opcode == ASL4 ||
		opcode == ASL5 ||
		opcode == DEC1 || 
		opcode == DEC2 || 
		opcode == DEC3 || 
		opcode == DEC4 || 
		opcode == INC1 || 
		opcode == INC2 || 
		opcode == INC3 || 
		opcode == INC4 || 
		opcode == LSR1 || 
		opcode == LSR2 || 
		opcode == LSR3 || 
		opcode == LSR4 || 
		opcode == LSR5 || 
		opcode == ROL1 ||
		opcode == ROL2 ||
		opcode == ROL3 ||
		opcode == ROL4 ||
		opcode == ROL5 ||
		opcode == ROR1 ||
		opcode == ROR2 ||
		opcode == ROR3 ||
		opcode == ROR4 ||
		opcode == ROR5) {
		clock->cycles += 2;
	    }
	    break;
	case ZERO_PAGE_X:
	    clock->cycles += 4;
	    if (opcode == ASL1 ||
		opcode == ASL2 ||
		opcode == ASL3 ||
		opcode == ASL4 ||
		opcode == ASL5 ||
		opcode == DEC1 || 
		opcode == DEC2 || 
		opcode == DEC3 || 
		opcode == DEC4 || 
		opcode == INC1 || 
		opcode == INC2 || 
		opcode == INC3 || 
		opcode == INC4 || 
		opcode == LSR1 || 
		opcode == LSR2 || 
		opcode == LSR3 || 
		opcode == LSR4 || 
		opcode == LSR5 || 
		opcode == ROL1 ||
		opcode == ROL2 ||
		opcode == ROL3 ||
		opcode == ROL4 ||
		opcode == ROL5 ||
		opcode == ROR1 ||
		opcode == ROR2 ||
		opcode == ROR3 ||
		opcode == ROR4 ||
		opcode == ROR5) {
		clock->cycles += 2;
	    }
	    break;
	case ABSOLUTE:
	    clock->cycles += 4;
	    if (opcode == ASL1 ||
		opcode == ASL2 ||
		opcode == ASL3 ||
		opcode == ASL4 ||
		opcode == ASL5 ||
		opcode == DEC1 || 
		opcode == DEC2 || 
		opcode == DEC3 || 
		opcode == DEC4 || 
		opcode == INC1 || 
		opcode == INC2 || 
		opcode == INC3 || 
		opcode == INC4 || 
		opcode == LSR1 || 
		opcode == LSR2 || 
		opcode == LSR3 || 
		opcode == LSR4 || 
		opcode == LSR5 || 
		opcode == JSR  ||
		opcode == ROL1 ||
		opcode == ROL2 ||
		opcode == ROL3 ||
		opcode == ROL4 ||
		opcode == ROL5 ||
		opcode == ROR1 ||
		opcode == ROR2 ||
		opcode == ROR3 ||
		opcode == ROR4 ||
		opcode == ROR5) {
		clock->cycles += 2;
	    }
	    if (opcode == JMP1 || opcode == JMP2) {
		clock->cycles -= 1;
	    }
	case ABSOLUTE_X:
	    clock->cycles += 4;
	    if (opcode == STA1 || opcode == STA2 ||
		opcode == STA3 || opcode == STA4 ||
		opcode == STA5 || opcode == STA6 ||
		opcode == STA7) {
		clock->cycles += 1;
	    }
	    else if (opcode == ASL1 ||
		opcode == ASL2 ||
		opcode == ASL3 ||
		opcode == ASL4 ||
		opcode == ASL5 ||
		opcode == DEC1 || 
		opcode == DEC2 || 
		opcode == DEC3 || 
		opcode == DEC4 || 
		opcode == INC1 || 
		opcode == INC2 || 
		opcode == INC3 || 
		opcode == INC4 || 
		opcode == LSR1 || 
		opcode == LSR2 || 
		opcode == LSR3 || 
		opcode == LSR4 || 
		opcode == LSR5 || 
		opcode == JSR  ||
		opcode == ROL1 ||
		opcode == ROL2 ||
		opcode == ROL3 ||
		opcode == ROL4 ||
		opcode == ROL5 ||
		opcode == ROR1 ||
		opcode == ROR2 ||
		opcode == ROR3 ||
		opcode == ROR4 ||
		opcode == ROR5) {
		clock->cycles += 3;
	    }
	    else if ((address & 0x00FF) == 0xFF) {
		//Page crossing
		clock->cycles += 1;
	    }
	    break;
	case ABSOLUTE_Y:
	    clock->cycles += 4;
	    if (opcode == STA1 || opcode == STA2 ||
		opcode == STA3 || opcode == STA4 ||
		opcode == STA5 || opcode == STA6 ||
		opcode == STA7) {
		clock->cycles += 1;
	    }
	    else if ((address & 0x00FF) == 0xFF) {
		//Page crossing
		clock->cycles += 1;
	    }
	    break;
	case INDIRECT_X:
	    clock->cycles += 6;
	    break;
	case INDIRECT_Y:
	    clock->cycles += 5;
	    if (opcode == STA1 || opcode == STA2 ||
		opcode == STA3 || opcode == STA4 ||
		opcode == STA5 || opcode == STA6 ||
		opcode == STA7) {
		clock->cycles += 1;
	    }
	    else if ((address & 0x00FF) == 0xFF) {
		//Page crossing
		clock->cycles += 1;
	    }
	    break;
	case RELATIVE:
	    clock->cycles += 2;
	    break;
	case INDIRECT:
	    clock->cycles += 5;
	    break;
	case IMPLIED:
	    clock->cycles += 2;
	    if (opcode == PHA || opcode == PHP) {
		clock->cycles += 1;
	    }
	    else if (opcode == PLA || opcode == PLP) {
		clock->cycles += 2;
	    }
	    else if (opcode == RTI || opcode == RTS) {
		clock->cycles += 4;
	    }
	    else if (opcode == BRK) {
		clock->cycles += 5;
	    }
	    break;
	default:
	    //SOMETHING WENT REALLY WRONG
	    clock->cycles += 1;
	    cout << "Timing unrecognized" << endl;
    }
}

uint32_t Cpu::read_operand(uint8_t opcode, addr_mode_t *addr_mode) {
    *addr_mode = IMPLIED;
    //obtenemos la columna de la matriz de opcodes
    uint8_t column = opcode & 0x1f;
    switch (column) {
	case 0x00:
	    if (opcode == JSR) 
		return read_operand_absolute(addr_mode);
	    else if (opcode != BRK && opcode != RTI && opcode != RTS) 
		return read_operand_imm(addr_mode);
	    break;
	case 0x01:
	    return read_operand_indexed_indirect(addr_mode);
	    break;
	case 0x04:
	    return read_operand_zero_page(addr_mode);
	    break;
	case 0x05:
	    return read_operand_zero_page(addr_mode);
	    break;
	case 0x06:
	    return read_operand_zero_page(addr_mode);
	    break;
	case 0x09:
	    return read_operand_imm(addr_mode);
	    break;
	case 0x0c:
	    if (opcode == JMP2) 
		return read_operand_indirect(addr_mode);
	    else return read_operand_absolute(addr_mode);
	    break;
	case 0x0d:
	    return read_operand_absolute(addr_mode);
	    break;
	case 0x0e:
	    return read_operand_absolute(addr_mode);
	    break;
	case 0x10:
	    return read_operand_relative(addr_mode);
	    break;
	case 0x11:
	    return read_operand_indirect_indexed(addr_mode);
	    break;
	case 0x14:
	    return read_operand_zero_page_indexed_x(addr_mode);
	    break;
	case 0x15:
	    return read_operand_zero_page_indexed_x(addr_mode);
	    break;
	case 0x16:
	    return read_operand_zero_page_indexed_x(addr_mode);
	    break;
	case 0x19:
	    return read_operand_absolute_indexed_y(addr_mode);
	    break;
	case 0x1c:
	    return read_operand_absolute_indexed_x(addr_mode);
	    break;
	case 0x1d:
	    return read_operand_absolute_indexed_x(addr_mode);
	    break;
	case 0x1e:
	    if (opcode == LDX5) 
		return read_operand_absolute_indexed_y(addr_mode);
	    else 
		return read_operand_absolute_indexed_x(addr_mode);
	    break;
	default:
	    regPC = regPC + 1; //No operand
	    return 0x00;
	    break;
    }
}

uint32_t Cpu::read_operand_indexed_indirect(addr_mode_t *addr_mode) {
    uint8_t operand;
    uint16_t address;
    uint8_t d = mem.read_byte(regPC + 0x0001);
    address = mem.read_word(uint16_t (0x00FF & (d+regX)));
    operand = mem.read_byte(address);
    regPC += 0x02;
    *addr_mode = INDIRECT_X;
    return (d << 16) | operand;
}

uint32_t Cpu::read_operand_zero_page(addr_mode_t *addr_mode) {
    uint8_t operand;
    uint16_t address;
    address = 0x0000;
    uint8_t d = mem.read_byte(regPC + 0x0001);
    operand = mem.read_byte(address | (0x00FF & d));
    regPC += 0x02;
    *addr_mode = ZERO_PAGE;
    return (d << 16) | operand;
}

uint32_t Cpu::read_operand_imm(addr_mode_t *addr_mode) {
    uint8_t operand;
    operand = mem.read_byte(regPC + 0x0001);
    regPC += 0x02;
    *addr_mode = IMMEDIATE;
    return operand;
}

uint32_t Cpu::read_operand_absolute(addr_mode_t *addr_mode) {
    uint16_t operand;
    uint8_t high, low;
    low = mem.read_byte(regPC + 0x0001);
    high = mem.read_byte(regPC + 0x0002);
    uint16_t address = (high << 8) | low;
    operand = mem.read_byte(address);
    regPC += 0x03;
    *addr_mode = ABSOLUTE;
    return (address << 16) | operand;
}

uint32_t Cpu::read_operand_indirect(addr_mode_t *addr_mode) {
    uint16_t pointer;
    uint8_t high, low, operand;
    low = mem.read_byte(regPC + 0x0001);
    high = mem.read_byte(regPC + 0x0002);
    pointer = mem.read_word((high << 8) | low);
    operand = mem.read_byte(pointer);
    regPC += 0x03;
    *addr_mode = INDIRECT;
    return (pointer << 16) | operand;
}

uint32_t Cpu::read_operand_relative(addr_mode_t *addr_mode) {
    uint8_t operand;
    operand = mem.read_byte(regPC+ 0x0001);
    uint16_t signExtOperand = (operand << 8) >> 8;
    regPC += 0x02;
    signExtOperand += regPC;
    *addr_mode = RELATIVE;
    return signExtOperand << 16;
}

uint32_t Cpu::read_operand_indirect_indexed(addr_mode_t *addr_mode) {
    uint16_t address;
    uint8_t operand;
    uint8_t high, low;
    low = mem.read_byte(regPC + 0x0001);
    high = mem.read_byte(regPC + 0x0002);
    address = mem.read_word((high << 8) | low);
    operand = mem.read_byte(address);
    operand += regY;
    regPC += 0x02;
    *addr_mode = INDIRECT_Y;
    return (address << 16) | operand;
}

uint32_t Cpu::read_operand_zero_page_indexed_x(addr_mode_t *addr_mode) {
    uint8_t operand;
    uint16_t address;
    uint8_t d = mem.read_byte(regPC + 0x0001);
    address = (0x0FF & (d+regX));
    operand = mem.read_byte(address | (0x00FF & (d+regX)));
    regPC += 0x02;
    *addr_mode = ZERO_PAGE_X;
    return (address << 16) | operand;
}

uint32_t Cpu::read_operand_absolute_indexed_x(addr_mode_t *addr_mode) {
    uint16_t address;
    uint8_t operand;
    uint8_t high, low;
    low = mem.read_byte(regPC + 0x0001);
    high = mem.read_byte(regPC + 0x0002);
    address = (high << 8) | low;
    address += regX;
    operand = mem.read_word(address);
    regPC += 0x03;
    *addr_mode = ABSOLUTE_X;
    return (address << 16) | operand;
}

uint32_t Cpu::read_operand_absolute_indexed_y(addr_mode_t *addr_mode) {
    uint16_t address;
    uint8_t operand;
    uint8_t high, low;
    low = mem.read_byte(regPC + 0x0001);
    high = mem.read_byte(regPC + 0x0002);
    address = (high << 8) | low;
    address += regY;
    operand = mem.read_byte(address);
    regPC += 0x03;
    *addr_mode = ABSOLUTE_Y;
    return (address << 16) | operand;
}

void Cpu::ADC_execute(uint8_t operand) {
    // Add with carry
    // A,Z,C,N = A+M+C
    uint16_t result = regA + operand + regP.C;
    bool signResult, signRegA, signOperand;

    signResult = (result >> 7) & 1; //Mask to eliminate the carry
    signRegA = regA >> 7;
    signOperand = operand >> 7;

    regA = result;

    regP.Z = (result & 0x00FF) == 0;
    regP.C = result >> 8;
    regP.V = ((signRegA == signOperand) && (signResult != signRegA));
    regP.N = signResult;
}

void Cpu::AND_execute(uint8_t operand) {
    // Logical AND
    // A,Z,N = A&M
    uint8_t result = regA & operand;

    regA = result;

    regP.Z = result == 0;
    regP.N = (result >> 7) & 1;
}

void Cpu::ASL_mem_execute(uint16_t address, uint8_t operand) {
    // Arithmetic shift left (Memory content)
    // M,Z,C,N = M*2

    uint16_t result = operand << 1;

    mem.write_byte(address, operand);

    regP.Z = (result & 0x00FF) == 0;
    regP.C = result >> 8;
    regP.N = (result >> 7) & 1;
}

void Cpu::ASL_A_execute() {
    // Arithmetic shift left (Accumulator content)
    // A,Z,C,N = A*2

    uint16_t result = regA << 1;

    regA = result;

    regP.Z = regA == 0;
    regP.C = result >> 8;
    regP.N = (result >> 7) & 1;
}

void Cpu::BCC_execute(uint16_t address) {
    // Branch if carry clear
    
    if (!regP.C) {
	regPC = address;
    }
}

void Cpu::BCS_execute(uint16_t address) {
    // Branch if carry set
    
    if (regP.C) {
	regPC = address;
    }
}

void Cpu::BEQ_execute(uint16_t address) {
    // Branch if equal

    if (regP.Z) {
	regPC = address;
    }
}

void Cpu::BIT_execute(uint8_t operand) {
    // Bit test
    // tmp = A & M, N = tmp[7], V = tmp[6]

    uint8_t tmp =regA & operand;

    regP.Z = tmp == 0;
    regP.V = (tmp >> 6) & 1;
    regP.N = (tmp >> 7) & 1;
}

void Cpu::BMI_execute(uint16_t address) {
    // Branch if minus
    
    if (regP.N) {
	regPC = address;
    }
}

void Cpu::BNE_execute(uint16_t address) {
    // Branch if not equal

    if (!regP.Z) {
	regPC = address;
    }
}

void Cpu::BPL_execute(uint16_t address) {
    // Branch if positive

    if (!regP.N) {
	regPC = address;
    }
}

void Cpu::BRK_execute() {
    // Force interrupt

    uint8_t lowPC, highPC;
    lowPC = regPC;
    highPC = (regPC >> 8);
    
    uint8_t procStat;
    procStat =  (regP.N) << 6 |
		(regP.V) << 5 |
		(regP.B) << 4 |
		(regP.D) << 3 |
		(regP.I) << 2 |
		(regP.Z) << 1 |
		(regP.C);
    
    //We push the PC and Processor Status into the stack
    pushStack(lowPC);
    pushStack(highPC);
    pushStack(procStat);

    //We set the PC to the interrupt vector and the Break command
    regPC = mem.read_word(BRK_INT_VECTOR_ADDR);
    regP.B = 1;
}

void Cpu::BVC_execute(uint16_t address) {
    // Branch if overflow clear

    if (!regP.V) {
	regPC = address;
    }
}

void Cpu::BVS_execute(uint16_t address) {
    //Branch if overflow set

    if (regP.V) {
	regPC = address;
    }
}

void Cpu::CLC_execute() {
    //Clear carry flag

    regP.C = 0;
}

void Cpu::CLD_execute() {
    //Clear decimal mode

    regP.D = 0;
}

void Cpu::CLI_execute() {
    //Clear interrupt disable

    regP.I = 0;
}

void Cpu::CLV_execute() {
    //Clear overflow flag

    regP.V = 0;
}

void Cpu::CMP_execute(uint8_t operand) {
    //Compare
    //Z,C,N = A-M

    uint8_t comp = regA - operand;

    regP.C = (int8_t) regA > (int8_t) operand;
    regP.Z = regA == operand;
    regP.N = comp >> 7;
}

void Cpu::CPX_execute(uint8_t operand) {
    //Compare X register
    //Z,C,N = X-M

    uint8_t comp = regX - operand;

    regP.C = (int8_t) regX > (int8_t) operand;
    regP.Z = regX == operand;
    regP.N = comp >> 7;
}   

void Cpu::CPY_execute(uint8_t operand) {
    //Compare Y register
    //Z,C,N = Y-M

    uint8_t comp = regY - operand;

    regP.C = (int8_t) regY > (int8_t) operand;
    regP.Z = regX == operand;
    regP.N = comp >> 7;
}

void Cpu::DEC_execute(uint16_t address, uint8_t operand) {
    //Decrement memory
    //M,Z,N = M-1

    uint8_t result = operand-1;
    mem.write_byte(address, result);

    regP.Z = result == 0;
    regP.N = result >> 7;
}

void Cpu::DEX_execute() {
    //Decrement X register
    //X,Z,N = X-1

    uint8_t result = regX-1;
    regX = result;

    regP.Z = result == 0;
    regP.N = result >> 7;
}

void Cpu::DEY_execute() {
    //Decrement Y register
    //Y,Z,N = Y-1

    uint8_t result = regY-1;
    regY = result;

    regP.Z = result == 0;
    regP.N = result >> 7;
}

void Cpu::EOR_execute(uint8_t operand) {
    //Exclusive OR
    //A,Z,N = A^M

    uint8_t result = regA ^ operand;
    regA = result;

    regP.Z = result == 0;
    regP.N = result >> 7;
}

void Cpu::INC_execute(uint16_t address, uint8_t operand) {
    //Increment memory
    //M,Z,N = M+1

    uint8_t result = operand + 1;
    mem.write_byte(address, result);

    regP.Z = result == 0;
    regP.N = result >> 7;
}

void Cpu::INX_execute() {
    //Increment X register
    //X,Z,N = X+1

    uint8_t result = regX + 1;
    regX = result;

    regP.Z = result == 0;
    regP.N = result >> 7;
}

void Cpu::INY_execute() {
    //Increment Y register
    //Y,Z,N = Y+1

    uint8_t result = regY + 1;
    regY = result;

    regP.Z = result == 0;
    regP.N = result >> 7;
}

void Cpu::JMP_execute(uint16_t address) {
    //Jump
    
    regPC = address;
}

void Cpu::JSR_execute(uint16_t address) {
    //Jump to subroutine
    
    uint16_t returnAddr = regPC - 4; //It must be PC - 1, and it already advanced 3
    pushStack(returnAddr); //low
    pushStack(returnAddr >> 8); //high
    regPC = address;
}

void Cpu::LDA_execute(uint8_t operand) {
    //Load accumulator
    //A,Z,N = M

    regA = operand;

    regP.Z = regA == 0;
    regP.N = regA >> 7;
}

void Cpu::LDX_execute(uint8_t operand) {
    //Load X register
    //X,Z,N = M

    regX = operand;

    regP.Z = regX == 0;
    regP.N = regX >> 7;
}

void Cpu::LDY_execute(uint8_t operand) {
    //Load Y register
    //Y,Z,N = M

    regY = operand;

    regP.Z = regY == 0;
    regP.N = regY >> 7;
}

void Cpu::LSR_mem_execute(uint16_t address, uint8_t operand) {
    // Logical shift right (Memory content)
    // M,Z,C,N = M/2

    uint8_t result = operand >> 1;

    mem.write_byte(address, operand);

    regP.Z = result == 0;
    regP.C = operand & 1;
    regP.N = result >> 7;
}

void Cpu::LSR_A_execute() {
    // Logical shift right (Accumulator content)
    // A,Z,C,N = A/2

    uint8_t result = regA >> 1;

    regP.Z = result == 0;
    regP.C = regA & 1;
    regP.N = result >> 7;

    regA = result;
}

void Cpu::NOP_execute() {
    // No operation
}

void Cpu::ORA_execute(uint8_t operand) {
    // Logical inclusive OR
    // A,Z,N = A|M

    uint8_t result = regA | operand;
    regA = result;

    regP.Z = result == 0;
    regP.N = result >> 7;
}

void Cpu::PHA_execute() {
    // Push accumulator
    pushStack(regA);
}

void Cpu::PHP_execute() {
    // Push processor status
    
    uint8_t procStat;
    procStat =  (regP.N) << 6 |
		(regP.V) << 5 |
		(regP.B) << 4 |
		(regP.D) << 3 |
		(regP.I) << 2 |
		(regP.Z) << 1 |
		(regP.C);
    pushStack(procStat);
}

void Cpu::PLA_execute() {
    // Pull accumulator

    regA = pullStack();

    regP.Z = regA == 0;
    regP.N = regA >> 7;
}

void Cpu::PLP_execute() {
    // Pull processor status

    uint8_t procStat = pullStack();
    regP.N = (procStat >> 6) & 1;
    regP.V = (procStat >> 5) & 1;
    regP.B = (procStat >> 4) & 1;
    regP.D = (procStat >> 3) & 1;
    regP.I = (procStat >> 2) & 1;
    regP.Z = (procStat >> 1) & 1;
    regP.C = procStat & 1;
}

void Cpu::ROL_mem_execute(uint16_t address, uint8_t operand) {
    // Rotate left (memory contents)

    uint16_t result = operand << 1 | regP.C;
    mem.write_byte(address, result);

    regP.C = result >> 8;
    regP.Z = (result & 0x00ff) == 0;
    regP.N = (result >> 7) & 1;
}

void Cpu::ROL_A_execute() {
    // Rotate Left (accumulator)

    uint16_t result = regA << 1 | regP.C;
    regA = result;

    regP.C = result >> 8;
    regP.Z = (result & 0x00ff) == 0;
    regP.N = (result >> 7) & 1;
}

void Cpu::ROR_mem_execute(uint16_t address, uint8_t operand) {
    // Rotate right (memory contents)

    uint8_t result = operand >> 1 | (regP.C << 7);
    mem.write_byte(address, result);

    regP.C = result & 1;
    regP.Z = result == 0;
    regP.N = result >> 7;
}

void Cpu::ROR_A_execute() {
    // Rotate right (accumulator)

    uint8_t result = regA >> 1 | (regP.C << 7);
    regA = result;

    regP.C = result & 1;
    regP.Z = result == 0;
    regP.N = result >> 7;
}

void Cpu::RTI_execute() {
    // Return from interrupt

    uint8_t procStat = pullStack();
    regP.N = (procStat >> 6) & 1;
    regP.V = (procStat >> 5) & 1;
    regP.B = (procStat >> 4) & 1;
    regP.D = (procStat >> 3) & 1;
    regP.I = (procStat >> 2) & 1;
    regP.Z = (procStat >> 1) & 1;
    regP.C = procStat & 1;

    uint8_t PC_high = pullStack();
    uint8_t PC_low = pullStack();
    regPC = (PC_high << 8) | PC_low;
}

void Cpu::RTS_execute() {
    // Return from subroutine

    uint8_t PC_high = pullStack();
    uint8_t PC_low = pullStack();
    uint16_t PC = (PC_high << 8) | PC_low;
    regPC = PC + 4; //It had PC - 1 and already advanced 3
}

void Cpu::SBC_execute(uint8_t operand) {
    // Subtract with carry
    // A,Z,C,N = A-M-(1-C)

    int16_t result = (int16_t)regA - (int16_t)operand - (int16_t)(1 - regP.C);
    bool signResult, signRegA, signOperand;

    signResult = (result >> 7) & 1; //Mask to eliminate the carry
    signRegA = regA >> 7;
    signOperand = operand >> 7;

    regA = result;

    regP.Z = (result & 0x00FF) == 0;
    regP.C = result >> 8;
    regP.V = ((signRegA == signOperand) && (signResult != signRegA));
    regP.N = signResult;
}

void Cpu::SEC_execute() {
    // Set carry flag

    regP.C = 1;
}

void Cpu::SED_execute() {
    // Set decimal flag
    
    regP.D = 1;
}

void Cpu::SEI_execute() {
    // Set interrupt disable flag

    regP.I = 1;
}

void Cpu::STA_execute(uint16_t address) {
    // Store accumulator
    // M=A

    mem.write_byte(address, regA);
}


void Cpu::STX_execute(uint16_t address) {
    // Store register X
    // M=X

    mem.write_byte(address, regX);
}


void Cpu::STY_execute(uint16_t address) {
    // Store register Y
    // M=Y

    mem.write_byte(address, regY);
}

void Cpu::TAX_execute() {
    // Transfer accumulator to X
    // X=A

    regX = regA;

    regP.Z = regX == 0;
    regP.N = regX >> 7;
}

void Cpu::TAY_execute() {
    // Transfer accumulator to Y
    // Y=A

    regY = regA;

    regP.Z = regY == 0;
    regP.N = regY >> 7;
}

void Cpu::TSX_execute() {
    // Transfer stack pointer to X
    // X=SP

    regX = regSP; //Only lower byte

    regP.Z = regX == 0;
    regP.N = regX >> 7;
}

void Cpu::TXA_execute() {
    // Transfer X to accumulator
    // A=X

    regA = regX;

    regP.Z = regA == 0;
    regP.N = regA >> 7;
}

void Cpu::TXS_execute() {
    // Transfer X to stack pointer
    // S=X

    regSP = (regSP & 0xff00) | regX;
}

void Cpu::TYA_execute() {
    // Transfer Y to accumulator
    // A = Y

    regA = regY;

    regP.Z = regA == 0;
    regP.N = regA >> 7;
}

void Cpu::NMI_execute() {
    debug_dump(0);
    // Non-Maskable interrupt
    cout << "NMI INTERRUPT" << endl;

    uint8_t lowPC, highPC;
    lowPC = regPC;
    highPC = (regPC >> 8);
    
    uint8_t procStat;
    procStat =  (regP.N) << 6 |
		(regP.V) << 5 |
		(regP.B) << 4 |
		(regP.D) << 3 |
		(regP.I) << 2 |
		(regP.Z) << 1 |
		(regP.C);
    
    //We push the PC and Processor Status into the stack
    pushStack(lowPC);
    pushStack(highPC);
    pushStack(procStat);

    //We set the PC to the interrupt vector
    regPC = mem.read_word(NMI_INT_VECTOR_ADDR);
}

void Cpu::pushStack(uint8_t data) {
    mem.write_byte(regSP, data);
    regSP--;
}

uint8_t Cpu::pullStack() {
    regSP++;
    return mem.read_byte(regSP);
}

void Cpu::debug_dump(uint8_t inst) {
    cout << "INSTRUCCION" << std::hex << (int) inst << endl;
    cout << "regA =  " << std::hex << (unsigned) regA << endl;
    cout << "regX =  " << std::hex << (unsigned) regX << endl;
    cout << "regY =  " << std::hex << (unsigned) regY << endl;
    cout << "regPC = " << std::hex << (unsigned) regPC << endl;
    cout << "regSP = " << std::hex << (unsigned) regSP << endl;
    cout << "regP =" << endl;
    cout << "    C = " << regP.C << endl;
    cout << "    Z = " << regP.Z << endl;
    cout << "    I = " << regP.I << endl;
    cout << "    D = " << regP.D << endl;
    cout << "    B = " << regP.B << endl;
    cout << "    V = " << regP.V << endl;
    cout << "    N = " << regP.N << endl;
    cout << "cycles = " << clock->cycles << endl;
}
