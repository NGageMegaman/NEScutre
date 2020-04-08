#include <stdint.h>
#include "cpu.h"
using namespace std;

cpu::cpu() {
    regX = regY = regA = regP = regSP = regCP = 0x00;
}
