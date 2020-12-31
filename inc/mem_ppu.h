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
	uint8_t read_PPUCTRL();
	void write_PPUCTRL(uint8_t data);
	void write_PPUADDR(uint8_t addr);
	uint8_t read_PPUDATA();
	void write_PPUDATA(uint8_t data);
	uint8_t read_PPUSTATUS();
	void write_OAMADDR(uint8_t addr);
	uint8_t read_OAMDATA();
	void write_OAMDATA(uint8_t data);
	void write_OAMDMA(uint8_t data[256]);
	uint8_t read_PPUMASK();
	void write_PPUMASK(uint8_t data);
	void write_PPUSCROLL(uint8_t data);

	void vblank_end();
	unsigned char *ram;
	unsigned char *oam;

	uint8_t PPUCTRL;
	uint16_t PPUADDR;
	uint8_t PPUDATA;
	uint8_t PPUSTATUS;
	uint16_t OAMADDR;
	uint8_t OAMDATA;
	uint8_t PPUMASK;
	uint16_t PPUSCROLL;
	bool ppuaddr_ptr_hl;
	bool oamaddr_ptr_hl;
	bool ppuscroll_ptr_hl;
	bool in_vblank;
};
