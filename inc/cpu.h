#include <stdint.h>
using namespace std;

class cpu {
    public:
	cpu();
    private:
	uint8_t regA, regX, regY, regP, regSP, regCP;
};
