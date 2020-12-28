#include <stdint.h>
#include <X11/Xlib.h>
#include "mem_ppu.h"
#ifndef CPU_H
#include "cpu.h"
#define CPU_H
#endif
using namespace std;

class Ppu {
    public:
	Ppu();
	void drawPixel(Display *di, Window wi, GC gc, int x, int y, int color);
	void draw();
	void vblank();
	Cpu *cpu;
	uint8_t PPUCTRL;
	uint8_t PPUMASK;
	uint8_t PPUSTATUS;
	uint8_t OAMADDR;
	uint8_t OAMDATA;
	uint8_t PPUSCROLL;
	uint8_t PPUADDR;
	uint8_t PPUDATA;
	uint8_t OAMDMA;
    private:
	Display *di;
	int sc;
	Window ro, wi;
	GC gc;
	Mem_ppu mem_ppu;
};
