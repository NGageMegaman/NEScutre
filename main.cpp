#include <stdint.h>
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
    unsigned int next_vblank = 89342;
    Clock nes_clock;
    Mem_ppu nes_mem_ppu;
    Cpu nes_cpu;
    nes_cpu.clock = &nes_clock;
    nes_cpu.mem.mem_ppu = &nes_mem_ppu;
    Ppu nes_ppu;
    nes_ppu.cpu = &nes_cpu;
    nes_ppu.mem_ppu = &nes_mem_ppu;
    nes_ppu.draw();
    cout << "lmao i drew" << endl;
    while(1) {
	    nes_cpu.execute();
	    if (nes_clock.cycles >= next_vblank) {
	        next_vblank += 89342;
	        nes_ppu.vblank();
	        nes_ppu.draw();
	    }
    }
}
