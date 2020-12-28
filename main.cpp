#include <stdint.h>
#include <iostream>
#ifndef CPU_H
#include "cpu.h"
#define CPU_H
#endif
#include "ppu.h"
using namespace std;

int main() {
    unsigned int next_vblank = 89342;
    Clock nes_clock;
    Cpu nes_cpu;
    nes_cpu.clock = &nes_clock;
    Ppu nes_ppu;
    nes_ppu.cpu = &nes_cpu;
    nes_ppu.draw();
    while(1) {
	nes_cpu.execute();
	if (nes_clock.cycles >= next_vblank) {
	    next_vblank += 89342;
	    nes_ppu.vblank();
	    nes_ppu.draw();
	}
    }
}
