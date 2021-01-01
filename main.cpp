#include <stdint.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#ifndef MEM_PPU_H
#include "mem_ppu.h"
#define MEM_PPU_H
#endif
#ifndef CPU_H
#include "cpu.h"
#define CPU_H
#endif
#ifndef PPU_H
#include "ppu.h"
#define PPU_H
#endif
using namespace std;

int main() {
    int count = 0, n_scanline;
    unsigned int next_vblank = 89342;
    unsigned int next_vblank_end = 89342 + 7746;
    unsigned int next_sprite = 89342 + 7746 + 340;
    Clock nes_clock;
    Mem_ppu nes_mem_ppu;
    nes_mem_ppu.load_rom("smb.nes");
    Cpu nes_cpu;
    nes_cpu.clock = &nes_clock;
    nes_cpu.mem.mem_ppu = &nes_mem_ppu;
    Ppu nes_ppu;
    nes_ppu.cpu = &nes_cpu;
    nes_ppu.mem_ppu = &nes_mem_ppu;
    nes_cpu.mem.di = nes_ppu.di;
    while(1) {
	    nes_cpu.execute();
	    if (nes_clock.cycles >= next_vblank) {
		//usleep(400000);
		count++;
		n_scanline = 0;
	        next_vblank += 89342;
		next_sprite = next_vblank_end + 340;
	        nes_ppu.vblank();
	    }
	    if (nes_clock.cycles >= next_vblank_end) {
		next_vblank_end = next_vblank + 7746;
		nes_mem_ppu.vblank_end();
		nes_mem_ppu.PPUSTATUS = nes_mem_ppu.PPUSTATUS & 0xbf;
	    }
	    //Draw for each scanline
	    //We assume the first sprite always does s0h
	    if (n_scanline != 0xee && nes_clock.cycles >= next_sprite) {
		nes_ppu.draw(n_scanline);
		nes_ppu.drawSprites(n_scanline);
		next_sprite = next_sprite + 340;
		n_scanline++;
	    }
	   
    }
}
