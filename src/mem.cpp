#include <stdint.h>
#include <vector>
#include "mem.h"
using namespace std;

Mem::Mem() {
    ram = vector<uint8_t> (RAM_SIZE);
    for (int i = 0; i<=RAM_SIZE; ++i) {
	ram[i] = 0x00;
    }
    ram[0] = 0x65;
    ram[1] = 0x04;
    ram[2] = 0x43;
    ram[4] = 0xDE;
}

uint8_t Mem::read_byte(uint16_t addr) {
    return ram[addr];
}

uint16_t Mem::read_word(uint16_t addr) {
    uint16_t result;
    uint8_t lower, upper;
    lower = ram[addr&0xFFFE];
    upper = ram[addr|0x0001];
    result = ((uint16_t) upper << 8) | (uint16_t) lower; //$HHLL
    return result;
}

void Mem::write_byte(uint16_t addr, uint8_t data) {
    ram[addr] = data;
}
