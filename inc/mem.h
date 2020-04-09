#include <stdint.h>
#include <vector>
using namespace std;

const uint8_t RAM_SIZE = 0xFF; 

class Mem {
    public:
	Mem();
	uint8_t read_byte(uint16_t addr);
	uint16_t read_word(uint16_t addr);
	void write_byte(uint16_t addr, uint8_t data);
    private:
	vector<uint8_t> ram;
};
