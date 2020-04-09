#include <stdint.h>
#include <iostream>
#include "cpu.h"
using namespace std;

Cpu::Cpu() {
    regX = regY = regA = regP = regCP = 0x00;
    regSP = 0x0000;
}

void Cpu::execute() {
    uint8_t opcode = mem.read_byte(regCP);
    cout << "interpretamos instruccion " << (int) opcode << endl;
    uint16_t operand = read_operand(opcode);

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

uint16_t Cpu::read_operand(uint8_t opcode) {
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

uint16_t Cpu::read_operand_indexed_indirect() {
    uint8_t operand;
    uint8_t d = mem.read_byte(regCP + 0x0001);
    operand = mem.read_byte(uint16_t (0x00FF & (d+regX)));
    regCP += 0x02;
    return operand;
}

uint16_t Cpu::read_operand_zero_page() {
    uint8_t operand;
    uint16_t address;
    address = 0x0000;
    uint8_t d = mem.read_byte(regCP + 0x0001);
    operand = mem.read_byte(address | (0x00FF & d));
    regCP += 0x02;
    return operand;
}

uint16_t Cpu::read_operand_imm() {
    uint8_t operand;
    operand = mem.read_byte(regCP + 0x0001);
    regCP += 0x02;
    return operand;
}

uint16_t Cpu::read_operand_absolute() {
    uint16_t operand;
    uint8_t high, low;
    low = mem.read_byte(regCP + 0x0001);
    high = mem.read_byte(regCP + 0x0002);
    operand = (high << 8) | low;
    regCP += 0x03;
    return operand;
}

uint16_t Cpu::read_operand_indirect() {
    uint16_t operand;
    uint8_t high, low;
    low = mem.read_byte(regCP + 0x0001);
    high = mem.read_byte(regCP + 0x0002);
    operand = mem.read_byte((high << 8) | low);
    regCP += 0x03;
    return operand;
}

uint16_t Cpu::read_operand_relative() {
    uint16_t operand;
    operand = mem.read_byte(regCP + 0x0001);
    regCP += 0x02;
    operand += regCP;
    return operand;
}

uint16_t Cpu::read_operand_indirect_indexed() {
    uint16_t operand;
    uint8_t high, low;
    low = mem.read_byte(regCP + 0x0001);
    high = mem.read_byte(regCP + 0x0002);
    operand = mem.read_byte((high << 8) | low);
    operand += regY;
    regCP += 0x02;
    return operand;
}

uint16_t Cpu::read_operand_zero_page_indexed_x() {
    uint8_t operand;
    uint16_t address;
    address = 0x0000;
    uint8_t d = mem.read_byte(regCP + 0x0001);
    operand = mem.read_byte(address | (0x00FF & (d+regX)));
    regCP += 0x02;
    return operand;
}

uint16_t Cpu::read_operand_absolute_indexed_x() {
    uint16_t operand;
    uint8_t high, low;
    low = mem.read_byte(regCP + 0x0001);
    high = mem.read_byte(regCP + 0x0002);
    operand = (high << 8) | low;
    operand += regX;
    regCP += 0x03;
    return operand;
}

uint16_t Cpu::read_operand_absolute_indexed_y() {
    uint16_t operand;
    uint8_t high, low;
    low = mem.read_byte(regCP + 0x0001);
    high = mem.read_byte(regCP + 0x0002);
    operand = (high << 8) | low;
    operand += regY;
    regCP += 0x03;
    return operand;
}

void Cpu::ADC_execute(uint16_t operand) {
    chivato(operand);
}

void Cpu::chivato(uint16_t operand) {
    cout << "ejecutada la instruccion ADC con operando " << (int) operand << endl;
}
