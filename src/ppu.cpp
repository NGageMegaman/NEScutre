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
    cout << "Inside ppu" << endl;
    di = XOpenDisplay(getenv("DISPLAY"));
    cout << "XOpenDisplay" << endl;    
    //Create window
    int x = 0, y = 0, width = 256, height = 240, border_width = 1;
    sc = DefaultScreen(di);
    cout << "Defaultscreen" << endl;
    ro = DefaultRootWindow(di);
    cout << "DefaultRootWindow" << endl;
    wi = XCreateSimpleWindow(di, ro, x, y, width, height, border_width, BlackPixel(di, sc), WhitePixel(di, sc));
    cout << "CreateSimpleWindow" << endl;
    gc = XCreateGC(di, ro, 0, NULL);
    cout << "XCreateGC" << endl;
    XMapWindow(di, wi); //Make window visible
    cout << "XMapWindow" << endl;
    XStoreName(di, wi, "Window");
    cout << "XStoreName" << endl;
    mem_ppu->load_rom("smb.nes");
    cout << "Load rom" << endl;
}

void Ppu::drawPixel(Display *di, Window wi, GC gc, int x, int y, int color) {
    //Prepare the window for drawing
    XSetForeground(di, gc, color);
    XDrawPoint(di, wi, gc, x, y);
}

void Ppu::draw() {
    //Select what events the window will listen to
    XSelectInput(di, wi, ExposureMask);
    //XEvent ev;
    unsigned int nametable;
    unsigned int idx = 0;
    unsigned int attribute_addr = 0;
    unsigned char attribute;
    unsigned int line_addr, h_offset;
    unsigned int nt_off;
    unsigned int patterntable;
    //int a = XNextEvent(di, &ev);
    //if (ev.type == Expose) {
	for (int i = 0; i<30; ++i) {
	    for (int j = 0; j<32; ++j) {
		    //TILE
		    for (int i2 = 0; i2<8; ++i2) {
		        for (int j2 = 0; j2<8; ++j2) {
		    	//Compute which nametable we use
		    	nt_off = (mem_ppu->PPUCTRL & 3);
		    	nametable = 0x2000 + (nt_off * 0x0400);

                //Compute which pattern table we use
                patterntable = (mem_ppu->PPUCTRL >> 4) & 1;
                //Fetch the index of the bg tile
		    	idx = (mem_ppu->ram[nametable+(i*32)+j]);
                //If we use the second pt, we add 256 to the index
		    	if (patterntable) idx += 256;
                //Each pattern is 16-byte wide. i2 is the vertical offset
		    	line_addr = (idx * 16) + i2;
                //j2 is the horizontal offset. We start from the MSB
		    	h_offset = (7-j2);

                //Compute the bitplane

                /*
                Each tile is formed by two bitplanes. The first one
                is followed by the second one. We fetch the line_addr byte,
                which contains the first bitplane of a given line, and the
                line_addr+8 byte, which contains the second one. Using
                h_offset we get the particular bit of the line that we want
                */

		    	unsigned char bitplane = 
		    	     (((mem_ppu->ram[line_addr]  )>>h_offset)&1) +
		    	    ((((mem_ppu->ram[line_addr+8])>>h_offset)&1)<<1);

                //Compute the attribute address of the tile
                //TODO: this can be done outside the tile render loop
		    	attribute_addr = nametable + 0x3c0;
                
                /*
                Each attribute contains 4 pallette indexes corresponding to
                a 2x2 tile portion of the screen. They are arranged, from
                MSB to LSB as such:

                bottom right, bottom left, top right, top left
                     7,6           5,4        3,2       1,0

                That means that each pattern contains the pallette of
                two different rows and two different columns. If the
                row is odd, we shift the attribute 4 bits right, and if the
                column is odd, we shift the attribute 2 bits right. This
                way we get the correct pallette index on the lower 2 bits.
                */

                //Determine the attribute address
		    	attribute_addr = attribute_addr + (i/2)*8;
		    	attribute_addr = attribute_addr + (j/2);
                //Fetch the attribute byte
		    	attribute = mem_ppu->ram[attribute_addr];
                //Arrange the lower 2 bits with the correct pallette index
		    	if (i % 2 == 1) {
		    	    attribute = attribute >> 4;
		    	}
		    	if (j % 2 == 1) {
		    	    attribute = attribute >> 2;
		    	}
                //We compute the final pallette index with the bitplane value
		    	attribute = (attribute << 2) + bitplane;
                //We fetch the color value
		    	char color = mem_ppu->ram[0x3f00 + attribute];

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

