#include <stdint.h>
#include "cpu.h"
using namespace std;

cpu::cpu() {
    regX = regY = regA = regP = regCP = 0x00;
    regSP = 0x0000;
}

void cpu::execute(uint8_t opcode) {
    switch (opcode) {
	default : 
	    regCP += 0x02;
	    break;
	
    }
}
