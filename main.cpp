#include <stdint.h>
#include <iostream>
#include "cpu.h"
using namespace std;

int main() {
    Cpu nes_cpu;
    uint8_t instr = ADC1;
    for (int i = 0; i < 8; ++i) {
	nes_cpu.execute();
	instr += 0x04;
    }
    cout << "SIMULATION ENDED\n";
}
