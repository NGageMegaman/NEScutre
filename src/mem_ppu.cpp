#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mem_ppu.h"
using namespace std;

Mem_ppu::Mem_ppu() {
    PPUCTRL = 0;
    PPUADDR = 0;
    PPUDATA = 0;
    PPUSTATUS = 0;
    OAMADDR = 0;
    OAMDATA = 0;
    ram = (unsigned char*) malloc(PPU_RAM_SIZE);
    oam = (unsigned char*) malloc(256);
    ppuaddr_ptr_hl = 0;
    oamaddr_ptr_hl = 0;
    ppuscroll_ptr_hl = 0;
    in_vblank = false;
}

uint8_t Mem_ppu::read_byte(uint16_t addr) {
    //Mirroring
    if (addr > 0x2fff && addr < 0x3f00)
	addr = addr - 0x1000;
    else if (addr > 0x3f1f && addr < 0x4000)
	addr = addr - 0x0020;

    return ram[addr];
}

uint16_t Mem_ppu::read_word(uint16_t addr) {
    //Mirroring
    if (addr > 0x2fff && addr < 0x3f00)
	addr = addr - 0x1000;
    else if (addr > 0x3f1f && addr < 0x4000)
	addr = addr - 0x0020;
    uint16_t result;
    uint8_t lower, upper;
    lower = ram[addr&0xFFFE];
    upper = ram[addr|0x0001];
    result = ((uint16_t) upper << 8) | (uint16_t) lower; //$HHLL
    return result;
}

void Mem_ppu::write_byte(uint16_t addr, uint8_t data) {
    //Mirroring
    if (addr > 0x2fff && addr < 0x3f00)
	addr = addr - 0x1000;
    else if (addr > 0x3f1f && addr < 0x4000)
	addr = addr - 0x0020;
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

    /* 4. CHR ROM data */
    rom.read((char *)ram, CHR_ROM_SIZE*y);
    rom.close();
}

uint8_t Mem_ppu::read_PPUCTRL() {
    return PPUCTRL;
}

void Mem_ppu::write_PPUCTRL(uint8_t data) {
    PPUCTRL = data;
    //Lower bits of ppustatus are the last written bits
    PPUSTATUS = (PPUSTATUS & 0xe0) | (data & 0x1f);
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
    //Lower bits of ppustatus are the last written bits
    PPUSTATUS = (PPUSTATUS & 0xe0) | (addr & 0x1f);
}

uint8_t Mem_ppu::read_PPUSTATUS() {
    uint8_t result = PPUSTATUS;
    //Clear the vblank status bit if we are not in vblank
    if (!in_vblank)
	PPUSTATUS = PPUSTATUS & 0x7f;
    //Clear the address latch
    PPUADDR = 0;
    ppuaddr_ptr_hl = 0;
    ppuscroll_ptr_hl = 0;
    oamaddr_ptr_hl = 0;
    return result;
}

uint8_t Mem_ppu::read_PPUDATA() {
    uint8_t result;
    if (PPUADDR > 0x3eff) {
	//When ppuaddr is at the pallettes, read normally
	result = read_byte(PPUADDR);
	PPUDATA = result;
    }
    else {
	//If not, then read the internal buffer
	result = PPUDATA;
	PPUDATA = read_byte(PPUADDR);
    }
    //The internal buffer updates anyways

    if (((PPUCTRL >> 2) & 1) == 0) {
	    PPUADDR += 1;
    }
    else {
	    PPUADDR += 32;
    }
    return result;
}

void Mem_ppu::write_PPUDATA(uint8_t data) {
    write_byte(PPUADDR, data);
    if (((PPUCTRL >> 2) & 1) == 0) {
	    PPUADDR += 1;
    }
    else {
	    PPUADDR += 32;
    }
    //Lower bits of ppustatus are the last written bits
    PPUSTATUS = (PPUSTATUS & 0xe0) | (data & 0x1f);
}

void Mem_ppu::write_OAMADDR(uint8_t addr) {
    if (oamaddr_ptr_hl == 0) {
	//Writing high byte
	OAMADDR = (addr << 8);
	OAMADDR &= 0x3FFF; //Upper adddresses are mirrored
	oamaddr_ptr_hl = 1;
    }
    else {
	//Writing low byte
	OAMADDR |= (addr & 0x00FF);
	oamaddr_ptr_hl = 0;
    }
    //Lower bits of ppustatus are the last written bits
    PPUSTATUS = (PPUSTATUS & 0xe0) | (addr & 0x1f);
}

uint8_t Mem_ppu::read_OAMDATA() {
    uint8_t result = ram[OAMADDR];
    if (!in_vblank)
	OAMADDR = OAMADDR+1;
    return result;
}

void Mem_ppu::write_OAMDATA(uint8_t data) {
    ram[OAMADDR] = data;
    OAMADDR = OAMADDR+1;
}

void Mem_ppu::write_OAMDMA(uint8_t data[256]) {
    for (int i = 0; i<256; ++i) {
	oam[i] = data[i];
    }
}

uint8_t Mem_ppu::read_PPUMASK() {
    uint8_t result = PPUMASK;
    return result;
}

void Mem_ppu::write_PPUMASK(uint8_t data) {
    PPUMASK = data;
}

void Mem_ppu::write_PPUSCROLL(uint8_t data) {
    if (ppuscroll_ptr_hl == 0) {
	//Writing high byte
	PPUSCROLL = (data << 8);
	ppuscroll_ptr_hl = 1;
    }
    else {
	//Writing low byte
	PPUSCROLL |= (data & 0x00FF);
	ppuscroll_ptr_hl = 0;
    }
    //Lower bits of ppustatus are the last written bits
    PPUSTATUS = (PPUSTATUS & 0xe0) | (data & 0x1f);
}


void Mem_ppu::vblank_end() {
    //Clear vblank 
    PPUSTATUS = PPUSTATUS & 0x7f;
    in_vblank = false;
}
