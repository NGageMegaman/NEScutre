#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mem.h"
using namespace std;

Mem::Mem() {
    //Mem init with 0s
    for (int i = 0; i<=RAM_SIZE; ++i) {
	ram[i] = 0x00;
    }
    
    load_rom("smb.nes");
}

uint8_t Mem::read_byte(uint16_t addr) {
    if (addr == 0x2007)
        return mem_ppu->read_PPUDATA();
    else
        return ram[addr];
}

uint16_t Mem::read_word(uint16_t addr) {
    uint16_t result;
    uint8_t lower, upper;
    lower = ram[addr];
    upper = ram[addr+1];
    result = ((uint16_t) upper << 8) | (uint16_t) lower; //$HHLL
    return result;
}

void Mem::write_byte(uint16_t addr, uint8_t data) {
    if (addr == 0x2000)
	mem_ppu->write_PPUCTRL(data);
    //else if (addr == 0x2001)
	    //ppu->PPUMASK = data;
    //else if (addr == 0x2002)
	    //ppu->PPUSTATUS = data;
    //else if (addr == 0x2003)
	    //ppu->OAMADDR = data;
    //else if (addr == 0x2004)
	    //ppu->OAMDATA = data;
    //else if (addr == 0x2005)
	    //ppu->PPUSCROLL = data;
    else if (addr == 0x2006)
	    mem_ppu->write_PPUADDR(data);
    else if (addr == 0x2007)
	    mem_ppu->write_PPUDATA(data);
    //else if (addr == 0x4014)
	    //ppu->OAMDMA = data;
    else
	    ram[addr] = data;
}

void Mem::load_rom(char *name) {

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
    rom.read((char *)ram, HEADER_SIZE);
    
    int x = ram[4];
    //int y = ram[5];

    /* 3. PRG ROM data */
    rom.read((char *)ram+HEADER_SIZE, PRG_ROM_SIZE*x);

    ram[0xfffc] = 0x00;
    ram[0xfffd] = 0x80;

    rom.close();
}

