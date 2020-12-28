#include <stdint.h>
#include "mem.h"
#include "clock.h"
using namespace std;

struct ProcStatus {
    bool C; // Carry Flag
    bool Z; // Zero Flag
    bool I; // Interrupt Disable
    bool D; // Decimal Mode Flag
    bool B; // Break Command
    bool V; // Overflow Flag
    bool N; // Negative Flag
};

class Cpu {
    public:
	Cpu();
	void execute();
	void add_clock_cycles(uint8_t opcode, uint16_t address, addr_mode_t addr_mode);
	uint32_t read_operand(uint8_t opcode, addr_mode_t *addr_mode);
	uint32_t read_operand_indexed_indirect(addr_mode_t *addr_mode);
	uint32_t read_operand_zero_page(addr_mode_t *addr_mode);
	uint32_t read_operand_imm(addr_mode_t *addr_mode);
	uint32_t read_operand_absolute(addr_mode_t *addr_mode);
	uint32_t read_operand_indirect(addr_mode_t *addr_mode);
	uint32_t read_operand_relative(addr_mode_t *addr_mode);
	uint32_t read_operand_indirect_indexed(addr_mode_t *addr_mode);
	uint32_t read_operand_zero_page_indexed_x(addr_mode_t *addr_mode);
	uint32_t read_operand_absolute_indexed_x(addr_mode_t *addr_mode);
	uint32_t read_operand_absolute_indexed_y(addr_mode_t *addr_mode);
	void ADC_execute(uint8_t operand);
	void AND_execute(uint8_t operand);
	void ASL_mem_execute(uint16_t address, uint8_t operand);
	void ASL_A_execute();
	void BCC_execute(uint16_t address);
	void BCS_execute(uint16_t address);
	void BEQ_execute(uint16_t address);
	void BIT_execute(uint8_t operand);
	void BMI_execute(uint16_t address);
	void BNE_execute(uint16_t address);
	void BPL_execute(uint16_t address);
	void BRK_execute();
	void BVC_execute(uint16_t address);
	void BVS_execute(uint16_t address);
	void CLC_execute();
	void CLD_execute();
	void CLI_execute();
	void CLV_execute();
	void CMP_execute(uint8_t operand);
	void CPX_execute(uint8_t operand);
	void CPY_execute(uint8_t operand);
	void DEC_execute(uint16_t address, uint8_t operand);
	void DEX_execute();
	void DEY_execute();
	void EOR_execute(uint8_t operand);
	void INC_execute(uint16_t address, uint8_t operand);
	void INX_execute();
	void INY_execute();
	void JMP_execute(uint16_t address);
	void JSR_execute(uint16_t address);
	void LDA_execute(uint8_t operand);
	void LDX_execute(uint8_t operand);
	void LDY_execute(uint8_t operand);
	void LSR_mem_execute(uint16_t address, uint8_t operand);
	void LSR_A_execute();
	void NOP_execute();
	void ORA_execute(uint8_t operand);
	void PHA_execute();
	void PHP_execute();
	void PLA_execute();
	void PLP_execute();
	void ROL_mem_execute(uint16_t address, uint8_t operand);
	void ROL_A_execute();
	void ROR_mem_execute(uint16_t address, uint8_t operand);
	void ROR_A_execute();
	void RTI_execute();
	void RTS_execute();
	void SBC_execute(uint8_t operand);
	void SEC_execute();
	void SED_execute();
	void SEI_execute();
	void STA_execute(uint16_t address);
	void STX_execute(uint16_t address);
	void STY_execute(uint16_t address);
	void TAX_execute();
	void TAY_execute();
	void TSX_execute();
	void TXA_execute();
	void TXS_execute();
	void TYA_execute();

	void NMI_execute();
	void pushStack(uint8_t data);
	uint8_t pullStack();
	void debug_dump(uint8_t inst);
	void endExecution();
	Clock *clock;
	Mem mem;
    private:
	uint8_t regA, regX, regY;
	uint16_t regPC, regSP;
	ProcStatus regP;
};
