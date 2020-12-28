#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mem_ppu.h"
using namespace std;

Mem_ppu::Mem_ppu() {
    char ram[PPU_RAM_SIZE];
}

uint8_t Mem_ppu::read_byte(uint16_t addr) {
    return ram[addr];
}

uint16_t Mem_ppu::read_word(uint16_t addr) {
    uint16_t result;
    uint8_t lower, upper;
    lower = ram[addr&0xFFFE];
    upper = ram[addr|0x0001];
    result = ((uint16_t) upper << 8) | (uint16_t) lower; //$HHLL
    return result;
}

void Mem_ppu::write_byte(uint16_t addr, uint8_t data) {
    ram[addr] = data;
}

void Mem_ppu::load_rom(char *name) {

    /*This rom loads .nes roms in iNES file format
    The iNES file format follows the following sections:
    
    1. Header (16 bytes)
    2. Trainer, if present(0 or 512 bytes)
    3. PRG ROM data (16384 * x bytes)
    4. CHR ROM data, if present (8192 * y bytes)
    5. PlayChoice INST-ROM, if present (0 or 8192 bytes)
    6. PlayChoice PROM, if present (16 bytes Data, 16 CounterOut)
    */

    ifstream rom;
    rom.open((char *)name, std::ios_base::binary);

    /* 1. HEADER
    The header format is as follows:
    0-3: Constant $4E $45 $53 $1A
    4:  Size of PRG ROM in 16KB units (x)
    5:  Size of CHR ROM in 8KB units (y)
    6:  Flags 6  - Mapper, mirroring, battery, trainer
    7:  Flags 7  - Mapper, VS/Playchoice, NES 2.0
    8:  Flags 8  - PRG-RAM size
    9:  Flags 9  - TV system
    10: Flags 10 - TV system, PRG-RAM presence
    11-15: unused padding
    */
    char header[HEADER_SIZE];
    rom.read(header, HEADER_SIZE);
    
    int x = header[4];
    int y = header[5];

    /* 3. PRG ROM data */
    char prg_rom[PRG_ROM_SIZE*x];
    rom.read(prg_rom, PRG_ROM_SIZE*x);
    cout << PRG_ROM_SIZE*x << endl;

    /* 4. CHR ROM data */
    rom.read((char *)ram, CHR_ROM_SIZE*y);

    for (int i = 0; i<NAMETABLE_SIZE; ++i) {
	if (i < 256)
	    ram[0x2000 + i] = i;
	else ram[0x2000 + i] = 255;
    }

    ram[0x3f00] = 63;
    ram[0x3f01] = 127;
    ram[0x3f02] = 191;
    ram[0x3f03] = 255;


    rom.close();
}

void Mem_ppu::write_PPUCTRL(uint8_t data) {
    PPUCTRL = data;
}

void Mem_ppu::write_PPUADDR(uint8_t addr) {
    if (ppuaddr_ptr_hl == 0) {
	//Writing high byte
	PPUADDR = (addr << 8);
	PPUADDR &= 0x3FFF; //Upper adddresses are mirrored
	ppuaddr_ptr_hl = 1;
    }
    else {
	//Writing low byte
	PPUADDR |= (addr & 0x00FF);
	ppuaddr_ptr_hl = 0;
    }
}

uint8_t Mem_ppu::read_PPUDATA() {
    uint8_t result = PPUDATA;
    PPUDATA = ram[PPUADDR];
    if (((PPUCTRL >> 1) & 1) == 0) {
	PPUADDR += 1;
    }
    else {
	PPUADDR += 32;
    }
}

void Mem_ppu::write_PPUDATA(uint8_t data) {
    ram[PPUADDR] = data;
}
