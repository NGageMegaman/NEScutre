#include <stdint.h>
#include <iostream>
#include "cpu.h"
using namespace std;

cpu::cpu() {
    regX = regY = regA = regP = regCP = 0x00;
    regSP = 0x0000;
}

void cpu::execute(uint8_t opcode) {
    cout << "interpretamos instruccion " << (int) opcode << endl;
    uint8_t operand = read_operand(opcode);

    switch (opcode) {
	case ADC1:
	    ADC_execute(operand);
	    break;
	case ADC2:
	    ADC_execute(operand);
	    break;
	case ADC3:
	    ADC_execute(operand);
	    break;
	case ADC4:
	    ADC_execute(operand);
	    break;
	case ADC5:
	    ADC_execute(operand);
	    break;
	case ADC6:
	    ADC_execute(operand);
	    break;
	case ADC7:
	    ADC_execute(operand);
	    break;
	case ADC8:
	    ADC_execute(operand);
	    break;
	default : 
	    regCP += 0x02;
	    break;
    }
}

uint8_t cpu::read_operand(uint8_t opcode) {
    //obtenemos la columna de la matriz de opcodes
    uint8_t column = opcode & 0x1f;
    switch (column) {
	case 0x00:
	    if (opcode == JSR) return read_operand_absolute();
	    else return read_operand_imm();
	    break;
	case 0x01:
	    return read_operand_indexed_indirect();
	    break;
	case 0x04:
	    return read_operand_zero_page();
	    break;
	case 0x05:
	    return read_operand_zero_page();
	    break;
	case 0x06:
	    return read_operand_zero_page();
	    break;
	case 0x09:
	    return read_operand_imm();
	    break;
	case 0x0c:
	    if (opcode == JMP2) return read_operand_indirect();
	    else return read_operand_absolute();
	    break;
	case 0x0d:
	    return read_operand_absolute();
	    break;
	case 0x0e:
	    return read_operand_absolute();
	    break;
	case 0x10:
	    return read_operand_relative();
	    break;
	case 0x11:
	    return read_operand_indirect_indexed();
	    break;
	case 0x14:
	    return read_operand_zero_page_indexed_x();
	    break;
	case 0x15:
	    return read_operand_zero_page_indexed_x();
	    break;
	case 0x16:
	    return read_operand_zero_page_indexed_x();
	    break;
	case 0x19:
	    return read_operand_absolute_indexed_y();
	    break;
	case 0x1c:
	    return read_operand_absolute_indexed_x();
	    break;
	case 0x1d:
	    return read_operand_absolute_indexed_x();
	    break;
	case 0x1e:
	    if (opcode == LDX) 
		return read_operand_absolute_indexed_y();
	    else 
		return read_operand_absolute_indexed_x();
	    break;
	default:
	    return 0x00;
	    break;
    }
}

uint8_t cpu::read_operand_indexed_indirect() {
    regCP += 0x02;
    return 0x01;
}

uint8_t cpu::read_operand_zero_page() {
    regCP += 0x02;
    return 0x02;
}

uint8_t cpu::read_operand_imm() {
    regCP += 0x02;
    return 0x03;
}

uint8_t cpu::read_operand_absolute() {
    regCP += 0x03;
    return 0x04;
}

uint8_t cpu::read_operand_indirect() {
    regCP += 0x03;
    return 0x05;
}

uint8_t cpu::read_operand_relative() {
    regCP += 0x02;
    return 0x06;
}

uint8_t cpu::read_operand_indirect_indexed() {
    regCP += 0x02;
    return 0x07;
}

uint8_t cpu::read_operand_zero_page_indexed_x() {
    regCP += 0x03;
    return 0x08;
}

uint8_t cpu::read_operand_absolute_indexed_x() {
    regCP += 0x02;
    return 0x09;
}

uint8_t cpu::read_operand_absolute_indexed_y() {
    regCP += 0x03;
    return 0x0a;
}

void cpu::ADC_execute(uint8_t operand) {
    chivato(operand);
}

void cpu::chivato(uint8_t operand) {
    cout << "ejecutada la instruccion ADC con operando " << (int) operand << endl;
}
