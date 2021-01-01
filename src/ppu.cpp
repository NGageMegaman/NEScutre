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
    XSelectInput(di, wi, KeyPressMask | KeyReleaseMask | ExposureMask);
    XAutoRepeatOff(di);
    gc = XCreateGC(di, ro, 0, NULL);
    XMapWindow(di, wi); //Make window visible
    XStoreName(di, wi, "NEScutre");
    initColors();
}

void Ppu::drawPixel(Display *di, Window wi, GC gc, int x, int y, int color) {
    //Prepare the window for drawing
    XSetForeground(di, gc, color);
    XDrawPoint(di, wi, gc, x, y);
}

void Ppu::checkSprite0Hit(int n_scanline) {
    int pos_y = mem_ppu->oam[0];
    
    if (n_scanline == pos_y) {
        if (pos_y == n_scanline) {
	    mem_ppu->PPUSTATUS |= 0x40;
	}
    }
}
	

void Ppu::drawSprites(int n_scanline) {
    //Select what events the window will listen to
    uint8_t index, attribute, palette;
    int pos_x, pos_y;
    unsigned int patterntable, line_addr, h_offset, large_sprite;
    bool flip_h, flip_v, priority;

    //Show sprites ppumask bit
    if ((mem_ppu->PPUMASK >> 4)&1) {
	for (int i = 0; i<64; ++i) {
	    pos_y     = mem_ppu->oam[(i*4) + 0];
	    index     = mem_ppu->oam[(i*4) + 1];
	    attribute = mem_ppu->oam[(i*4) + 2];
	    pos_x     = mem_ppu->oam[(i*4) + 3];

	    if (n_scanline == pos_y) {

	    	palette = attribute & 3;
	    	priority = (attribute >> 5) & 1;
	    	flip_h = (attribute >> 6) & 1;
	    	flip_v = (attribute >> 7) & 1;

	    	large_sprite = (mem_ppu->PPUCTRL >> 5) & 1;
	    	if (large_sprite) {
	    	    patterntable = index & 1;
	    	    index = index & 0xfe;
	    	}
	    	else {
	    	    patterntable = (mem_ppu->PPUCTRL >> 3) & 1;
	    	}

	    	if (patterntable)
	    	    index += 256;

	    	if (pos_x > 8 || ((mem_ppu->PPUMASK >> 2)&1)) {
	    	    for (int i2 = 0; i2<8; ++i2) {
	    	        for (int j2 = 0; j2<8; ++j2) {
			    line_addr = (index * 16) + i2;
	    	            h_offset = (7-j2);
	    	            unsigned char bitplane =
	    	                (((mem_ppu->ram[line_addr])>>h_offset)&1) +
	    	               ((((mem_ppu->ram[line_addr+8])>>h_offset)&1)<<1);
            	            
	    	            unsigned char color = mem_ppu->ram[0x3f10 + (palette << 2) + bitplane];
	    	    	    uint8_t tmp_pos_x;
	    	    	    uint8_t tmp_pos_y;
	    	    	    if (flip_h)
	    	    	        tmp_pos_x = pos_x + 7 - j2;
	    	    	    else
	    	    	        tmp_pos_x = pos_x + j2;
	    	    	    if (flip_v)
	    	    	        tmp_pos_y = pos_y + 7 - i2;
	    	    	    else
	    	    	        tmp_pos_y = pos_y + i2;

			    int position = (tmp_pos_y*256) + tmp_pos_x;
			    if (bitplane != 0 && position < 61440) {
				if (priority == 0) {
				    mem_ppu->vram[position] = colors[color];
				}
				else if (mem_ppu->vram[position] == colors[mem_ppu->ram[0x3f00]]) {
				    mem_ppu->vram[position] = colors[color];
				}
			    }
	    	        }
	    	    }
	    	}

	    	if (large_sprite) {
	    	    index += 1;
	    	    pos_y += 16;
	    	    if (pos_x > 8 || ((mem_ppu->PPUMASK >> 2)&1)) {
	    	        for (int i2 = 0; i2<8; ++i2) {
	    		    for (int j2 = 0; j2<8; ++j2) {
				line_addr = (index * 16) + i2;
	    		        h_offset = (7-j2);
	    		        unsigned char bitplane =
	    		            (((mem_ppu->ram[line_addr])>>h_offset)&1) +
	    		           ((((mem_ppu->ram[line_addr+8])>>h_offset)&1)<<1);
            		            
	    		        unsigned char color = mem_ppu->ram[0x3f00 + (palette << 2) + 0x04 + bitplane];
			        uint8_t tmp_pos_x;
			        uint8_t tmp_pos_y;
	    	    	        if (flip_h)
				    tmp_pos_x = pos_x + 7 - j2;
	    	    	        else
				    tmp_pos_x = pos_x + j2;
	    	    	        if (flip_v)
				    tmp_pos_y = pos_y + 7 - i2;
	    	    	        else
				    tmp_pos_y = pos_y + i2;

				int position = (tmp_pos_y*256) + tmp_pos_x;

			    	if (bitplane != 0 && position < 61440) {
			    	    if (priority == 0) {
					mem_ppu->vram[position] = colors[color];
			    	    }
			    	    else if (mem_ppu->vram[position] == colors[mem_ppu->ram[0x3f00]]) {
					mem_ppu->vram[position] = colors[color];
			    	    }
			    	}
	    		    }
	    		}
	    	    }
	    	}
	    }
	}
    }
}

void Ppu::draw(int n_scanline) {
    //Select what events the window will listen to
    //XEvent ev;
    unsigned int nametable;
    unsigned int idx = 0;
    unsigned int attribute_addr = 0;
    unsigned char attribute;
    unsigned int line_addr, h_offset;
    unsigned int nt_off;
    unsigned int patterntable;
    int pos_x, pos_y, sc_off_x, sc_off_y;
    //int a = XNextEvent(di, &ev);
    //if (ev.type == Expose) {
    //BG enable
    if ((mem_ppu->PPUMASK >> 3)&1) {
	if ((n_scanline & 0x7) == 0) {
	    int i = n_scanline/8;
	    //Leftmost tiles
	    if (i > 0 || ((mem_ppu->PPUMASK >> 1)&1)) {
		for (int j = 0; j<32; ++j) {
	    	    //TILE
	    	    for (int i2 = 0; i2<8; ++i2) {
			for (int j2 = 0; j2<8; ++j2) {

			    //Compute the scrolling tile offsets
			    sc_off_y = mem_ppu->PPUSCROLL & 0x00ff;
			    sc_off_x = ((mem_ppu->PPUSCROLL & 0xff00)>>8) & 0x00ff;

			    //Compute which nametable we use
			    nt_off = (mem_ppu->PPUCTRL & 3);

			    //Compute the position
			    pos_x = (j*8)+j2 - sc_off_x;
			    if (pos_x < 0) {
				pos_x += 32*8;
				nt_off = (nt_off & 2) | ((nt_off+1) & 1);
			    }
			    pos_y = (i*8)+i2 - sc_off_y;
			    if (pos_y < 0) {
				pos_y += 30*8;
				nt_off = ((nt_off+2) & 2) | (nt_off & 1);
			    }

			    nametable = 0x2000 + (nt_off * 0x0400);

			    //Compute which pattern table we use
			    patterntable = (mem_ppu->PPUCTRL >> 4) & 1;
			    //Fetch the index of the bg tile
			    idx = (mem_ppu->ram[nametable+(i*32)+j]);
			    //If we use the second pt, we add 256 to the index
			    if (patterntable) 
				idx += 256;
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
	    	    	     (((mem_ppu->ram[line_addr]  )>>h_offset)&1) |
	    	    	    ((((mem_ppu->ram[line_addr+8])>>h_offset)&1)<<1);

            	    	    //Compute the attribute address of the tile
            	    	    //TODO: this can be done outside the tile render loop
	    	    	    attribute_addr = nametable + 0x03c0;
            	    	    
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
	    	            attribute_addr = attribute_addr + (i/4)*8;
	    	            attribute_addr = attribute_addr + (j/4);
	    	    	    //Fetch the attribute byte
	    	            attribute = mem_ppu->ram[attribute_addr];
	    	    	    //Arrange the lower 2 bits with the correct pallette index
	    	            if ((i & 3) > 1) {
	    	                attribute = attribute >> 4;
	    	            }
	    	            if ((j & 3) > 1) {
	    	                attribute = attribute >> 2;
	    	            }
	    	    	    //We compute the final pallette index with the bitplane value
	    	            attribute = ((attribute << 2) | bitplane) & 0x000f;
			    if (bitplane == 0) attribute = 0;
	    	    	    //We fetch the color value
	    	            unsigned char color = mem_ppu->ram[0x3f00 + attribute];
			    int position = (pos_y*256)+pos_x;
			    if (position < 61440)
				mem_ppu->vram[position] = colors[color];
			}
		    }
	    	}
	    }
	}
    }
}

void Ppu::drawScreen() {
    for (int i = 0; i<61440; ++i)
	drawPixel(di, wi, gc, (i%256), (i/256), mem_ppu->vram[i]);
}

void Ppu::vblank() {
    //Set the vblank bit
    mem_ppu->PPUSTATUS = mem_ppu->PPUSTATUS | 0x80;
    mem_ppu->in_vblank = true;
    if ((mem_ppu->PPUCTRL >> 7) & 1)
	cpu->NMI_execute();
}

void Ppu::initColors() {
    colors[0]  = (84  << 16) | (84  << 8 ) | 84;
    colors[1]  = (0   << 16) | (30  << 8 ) | 116;
    colors[2]  = (8   << 16) | (16  << 8 ) | 144;
    colors[3]  = (48  << 16) | (0   << 8 ) | 136;
    colors[4]  = (68  << 16) | (0   << 8 ) | 100;
    colors[5]  = (92  << 16) | (0   << 8 ) | 48;
    colors[6]  = (84  << 16) | (4   << 8 ) | 0;
    colors[7]  = (60  << 16) | (24  << 8 ) | 0;
    colors[8]  = (32  << 16) | (42  << 8 ) | 0;
    colors[9]  = (8   << 16) | (58  << 8 ) | 0;
    colors[10] = (0   << 16) | (64  << 8 ) | 0;
    colors[11] = (0   << 16) | (60  << 8 ) | 0;
    colors[12] = (0   << 16) | (50  << 8 ) | 0;
    colors[13] = (0   << 16) | (0   << 8 ) | 0;
    colors[14] = (0   << 16) | (0   << 8 ) | 0;
    colors[15] = (0   << 16) | (0   << 8 ) | 0;
    colors[16] = (152 << 16) | (150 << 8 ) | 152;
    colors[17] = (8   << 16) | (76  << 8 ) | 196;
    colors[18] = (48  << 16) | (50  << 8 ) | 236;
    colors[19] = (92  << 16) | (30  << 8 ) | 228;
    colors[20] = (136 << 16) | (20  << 8 ) | 176;
    colors[21] = (160 << 16) | (20  << 8 ) | 100;
    colors[22] = (152 << 16) | (34  << 8 ) | 32;
    colors[23] = (120 << 16) | (60  << 8 ) | 0;
    colors[24] = (84  << 16) | (90  << 8 ) | 0;
    colors[25] = (40  << 16) | (114 << 8 ) | 0;
    colors[26] = (8   << 16) | (124 << 8 ) | 0;
    colors[27] = (0   << 16) | (118 << 8 ) | 40;
    colors[28] = (0   << 16) | (102 << 8 ) | 120;
    colors[29] = (0   << 16) | (0   << 8 ) | 0;
    colors[30] = (0   << 16) | (0   << 8 ) | 0;
    colors[31] = (0   << 16) | (0   << 8 ) | 0;
    colors[32] = (236 << 16) | (238 << 8 ) | 236;
    colors[33] = (76  << 16) | (154 << 8 ) | 236;
    colors[34] = (120 << 16) | (124 << 8 ) | 236;
    colors[35] = (176 << 16) | (98  << 8 ) | 236;
    colors[36] = (228 << 16) | (84  << 8 ) | 236;
    colors[37] = (236 << 16) | (88  << 8 ) | 180;
    colors[38] = (236 << 16) | (106 << 8 ) | 100;
    colors[39] = (212 << 16) | (136 << 8 ) | 32;
    colors[40] = (160 << 16) | (170 << 8 ) | 0;
    colors[41] = (116 << 16) | (196 << 8 ) | 0;
    colors[42] = (76  << 16) | (208 << 8 ) | 32;
    colors[43] = (56  << 16) | (204 << 8 ) | 108;
    colors[44] = (56  << 16) | (180 << 8 ) | 204;
    colors[45] = (60  << 16) | (60  << 8 ) | 60;
    colors[46] = (0   << 16) | (0   << 8 ) | 0;
    colors[47] = (0   << 16) | (0   << 8 ) | 0;
    colors[48] = (236 << 16) | (238 << 8 ) | 236;
    colors[49] = (168 << 16) | (204 << 8 ) | 236;
    colors[50] = (188 << 16) | (188 << 8 ) | 236;
    colors[51] = (212 << 16) | (178 << 8 ) | 236;
    colors[52] = (236 << 16) | (174 << 8 ) | 236;
    colors[53] = (236 << 16) | (174 << 8 ) | 212;
    colors[54] = (236 << 16) | (180 << 8 ) | 176;
    colors[55] = (228 << 16) | (196 << 8 ) | 144;
    colors[56] = (204 << 16) | (210 << 8 ) | 120;
    colors[57] = (180 << 16) | (222 << 8 ) | 120;
    colors[58] = (168 << 16) | (226 << 8 ) | 144;
    colors[59] = (152 << 16) | (226 << 8 ) | 180;
    colors[60] = (160 << 16) | (214 << 8 ) | 228;
    colors[61] = (160 << 16) | (162 << 8 ) | 160;
    colors[62] = (0   << 16) | (0   << 8 ) | 0;
    colors[63] = (0   << 16) | (0   << 8 ) | 0;
}
