#include <stdint.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
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
    //Mirroring
    if (addr > 0x07FF) {
	if (addr < 0x1000)
	    addr -= 0x0800;
	else if (addr < 0x1800)
	    addr -= 0x1000;
	else if (addr < 0x2000)
	    addr -= 0x1800;
    }
    //PPU regs mirroring
    if (addr > 0x2007 && addr < 0x4000) {
	addr = (addr & 7) + 0x2000;
    }

    if (addr == 0x2000)
	return mem_ppu->read_PPUCTRL();
    else if (addr == 0x2001)
	return mem_ppu->read_PPUMASK();
    else if (addr == 0x2002)
	return mem_ppu->read_PPUSTATUS();
    else if (addr == 0x2007)
        return mem_ppu->read_PPUDATA();
    else if (addr == 0x4016) {
	uint8_t result = controller_latch & 1;
	controller_latch = controller_latch >> 1;
	controller_latch |= 0x80;
	return result;
    }
    else
        return ram[addr];
}

uint16_t Mem::read_word(uint16_t addr) {
    //Mirroring
    if (addr > 0x07FF) {
	if (addr < 0x1000)
	    addr -= 0x0800;
	else if (addr < 0x1800)
	    addr -= 0x1000;
	else if (addr < 0x2000)
	    addr -= 0x1800;
    }
    //PPU regs mirroring
    if (addr > 0x2007 && addr < 0x4000) {
	addr = (addr & 7) + 0x2000;
    }
    uint16_t result;
    uint8_t lower, upper;
    lower = ram[addr];
    upper = ram[addr+1];
    result = ((uint16_t) upper << 8) | (uint16_t) lower; //$HHLL
    return result;
}

void Mem::write_byte(uint16_t addr, uint8_t data) {
    //Mirroring
    if (addr > 0x07FF) {
	if (addr < 0x1000)
	    addr -= 0x0800;
	else if (addr < 0x1800)
	    addr -= 0x1000;
	else if (addr < 0x2000)
	    addr -= 0x1800;
    }
    //PPU regs mirroring
    if (addr > 0x2007 && addr < 0x4000) {
	addr = (addr & 7) + 0x2000;
    }
    
    if (addr == 0x2000)
	mem_ppu->write_PPUCTRL(data);
    else if (addr == 0x2001)
	mem_ppu->write_PPUMASK(data);
    //else if (addr == 0x2002)
	//ppu->PPUSTATUS = data;
    else if (addr == 0x2003)
	mem_ppu->write_OAMADDR(data);
    else if (addr == 0x2004)
	mem_ppu->write_OAMDATA(data);
    else if (addr == 0x2005)
	mem_ppu->write_PPUSCROLL(data);
    else if (addr == 0x2006)
	mem_ppu->write_PPUADDR(data);
    else if (addr == 0x2007)
	mem_ppu->write_PPUDATA(data);
    else if (addr == 0x4014) {
	uint8_t data_array[256];
	for (int i = 0; i < 256; ++i) 
	    data_array[i] = ram[(unsigned) (data << 8) + i];
	    
	mem_ppu->write_OAMDMA(data_array);
    }
    else if (addr == 0x4016 && data == 1) {
	controller_read_latch = 0;
	controller_latch = 0;
	XEvent event;
	while (XPending(di)) {
	    XNextEvent(di, &event);
	    int a;
	    if (event.type == KeyRelease) {
		a = event.xkey.keycode;
		if (a == 52)       //z_key
	    	    controller_inputs &= 0xfe;
	    	else if (a == 53)  //x_key
	    	    controller_inputs &= 0xfd;
	    	else if (a == 36)  //enter_key
	    	    controller_inputs &= 0xfb;
	    	else if (a == 22)  //backspace_key
	    	    controller_inputs &= 0xf7;
	    	else if (a == 111)  //up_key
	    	    controller_inputs &= 0xef;
	    	else if (a == 116)  //down_key
	    	    controller_inputs &= 0xdf;
	    	else if (a == 113)  //left_key
	    	    controller_inputs &= 0xbf;
	    	else if (a == 114)  //right_key
	    	    controller_inputs &= 0x7f;
	    }
	    if (event.type == KeyPress) {
		a = event.xkey.keycode;
		if (a == 52)       //z_key
	    	    controller_inputs |= 0x01;
	    	else if (a == 53)  //x_key
	    	    controller_inputs |= 0x02;
	    	else if (a == 36)  //enter_key
	    	    controller_inputs |= 0x04;
	    	else if (a == 22)  //backspace_key
	    	    controller_inputs |= 0x08;
	    	else if (a == 111)  //up_key
	    	    controller_inputs |= 0x10;
	    	else if (a == 116)  //down_key
	    	    controller_inputs |= 0x20;
	    	else if (a == 113)  //left_key
	    	    controller_inputs |= 0x40;
	    	else if (a == 114)  //right_key
	    	    controller_inputs |= 0x80;
	    }
	}
	controller_latch = controller_inputs;
    }
    else if (addr == 0x4016 && data == 0) {
	controller_read_latch = 1;
    }
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
    rom.read((char *)ram+0x8000, PRG_ROM_SIZE*x);

    ram[0xfffc] = 0x00;
    ram[0xfffd] = 0x80;

    rom.close();
}

