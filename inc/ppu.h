#include <stdint.h>
#include <X11/Xlib.h>
#ifndef MEM_PPU_H
#include "mem_ppu.h"
#define MEM_PPU_H
#endif
#ifndef CPU_H
#include "cpu.h"
#define CPU_H
#endif
using namespace std;

class Ppu {
    public:
	Ppu();
	void drawPixel(Display *di, Window wi, GC gc, int x, int y, int color);
	void draw(int n_scanline);
	void drawSprites(int n_scanline);
	void vblank();
	void initColors();
	Cpu *cpu;
	Mem_ppu *mem_ppu;
    private:
	Display *di;
	int sc;
	Window ro, wi;
	GC gc;
	uint32_t colors[64];
};
