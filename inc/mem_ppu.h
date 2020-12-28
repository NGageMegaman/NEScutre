#include <stdint.h>
#include <string>
#include <vector>
#ifndef DEFINES_H
#include "defines.h"
#define DEFINES_H
#endif
using namespace std;

class Mem_ppu {
    public:
	Mem_ppu();
	uint8_t read_byte(uint16_t addr);
	uint16_t read_word(uint16_t addr);
	void write_byte(uint16_t addr, uint8_t data);
	void load_rom(char *name);
	unsigned char ram[PPU_RAM_SIZE];
};
