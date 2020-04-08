#include <stdint.h>
using namespace std;

class cpu {
    public:
	cpu();
	void execute(uint8_t opcode);
    private:
	uint8_t regA, regX, regY, regP, regCP;
	uint16_t regSP;
};
