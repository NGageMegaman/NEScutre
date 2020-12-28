#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <X11/Xlib.h>
#include "ppu.h"
using namespace std;

Ppu::Ppu() {
    di = XOpenDisplay(getenv("DISPLAY"));
    
    //Create window
    int x = 0, y = 0, width = 256, height = 240, border_width = 1;
    sc = DefaultScreen(di);
    ro = DefaultRootWindow(di);
    wi = XCreateSimpleWindow(di, ro, x, y, width, height, border_width, BlackPixel(di, sc), WhitePixel(di, sc));
    gc = XCreateGC(di, ro, 0, NULL);
    XMapWindow(di, wi); //Make window visible
    XStoreName(di, wi, "Window");
    mem_ppu.load_rom("smb.nes");

}

void Ppu::drawPixel(Display *di, Window wi, GC gc, int x, int y, int color) {
    //Prepare the window for drawing
    XSetForeground(di, gc, color);
    XDrawPoint(di, wi, gc, x, y);
}

void Ppu::draw() {
    //Select what events the window will listen to
    XSelectInput(di, wi, ExposureMask);
    XEvent ev;
    int quit = 0;
    unsigned int idx = 0;
    unsigned int pallette_idx = 0;
    unsigned int attribute_idx = 0;
    unsigned char attribute;
    int offst = 7;
    int tiles = 0;
    //int a = XNextEvent(di, &ev);
    //if (ev.type == Expose) {
	for (int i = 0; i<30; ++i) {
	    for (int j = 0; j<32; ++j) {
		//TILE
		for (int i2 = 0; i2<8; ++i2) {
		    for (int j2 = 0; j2<8; ++j2) {
			unsigned int nametable = 0x2000;
			unsigned int nt_off;
			nt_off = (PPUCTRL & 3);
			nametable += (nt_off * 0x0400);
			idx = (mem_ppu.ram[nametable+(i*32)+j]);
			if ((PPUCTRL >> 4)&1) idx += 256;
			idx = (idx * 16) + i2;
			offst = (7-j2);
			unsigned char bitplane = 
			    ((mem_ppu.ram[idx])>>offst)&1 +
			    (((mem_ppu.ram[idx+8])>>offst)&1)*2;
			attribute_idx = 0x23c0;
			attribute_idx = attribute_idx + (i>>1)*8;
			attribute_idx = attribute_idx + (j>>1);
			attribute = mem_ppu.ram[attribute_idx];
			if (i % 2 == 0) {
			    attribute >> 4;
			}
			if (j % 2 == 0) {
			    attribute >> 2;
			}
			attribute += bitplane; // dir to color
			char color = mem_ppu.ram[0x3f00 + attribute];

			drawPixel(di, wi, gc, (j*8)+j2, (i*8)+i2, color);
		    }
		}
	    }
	}
    //}
}

void Ppu::vblank() {
    cpu->NMI_execute();
}

