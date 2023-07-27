#include "6502.h"
#include "../Bus.h"

CPU6502::CPU6502()
{
  using cpu = CPU6502;
  this->lookup = 	{
		{ "BRK", &cpu::BRK, &cpu::IMM, 7 },{ "ORA", &cpu::ORA, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 3 },{ "ORA", &cpu::ORA, &cpu::ZP0, 3 },{ "ASL", &cpu::ASL, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PHP", &cpu::PHP, &cpu::IMP, 3 },{ "ORA", &cpu::ORA, &cpu::IMM, 2 },{ "ASL", &cpu::ASL, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ORA", &cpu::ORA, &cpu::ABS, 4 },{ "ASL", &cpu::ASL, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BPL", &cpu::BPL, &cpu::REL, 2 },{ "ORA", &cpu::ORA, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ORA", &cpu::ORA, &cpu::ZPX, 4 },{ "ASL", &cpu::ASL, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "CLC", &cpu::CLC, &cpu::IMP, 2 },{ "ORA", &cpu::ORA, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ORA", &cpu::ORA, &cpu::ABX, 4 },{ "ASL", &cpu::ASL, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "JSR", &cpu::JSR, &cpu::ABS, 6 },{ "AND", &cpu::AND, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "BIT", &cpu::BIT, &cpu::ZP0, 3 },{ "AND", &cpu::AND, &cpu::ZP0, 3 },{ "ROL", &cpu::ROL, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PLP", &cpu::PLP, &cpu::IMP, 4 },{ "AND", &cpu::AND, &cpu::IMM, 2 },{ "ROL", &cpu::ROL, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "BIT", &cpu::BIT, &cpu::ABS, 4 },{ "AND", &cpu::AND, &cpu::ABS, 4 },{ "ROL", &cpu::ROL, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BMI", &cpu::BMI, &cpu::REL, 2 },{ "AND", &cpu::AND, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "AND", &cpu::AND, &cpu::ZPX, 4 },{ "ROL", &cpu::ROL, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "SEC", &cpu::SEC, &cpu::IMP, 2 },{ "AND", &cpu::AND, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "AND", &cpu::AND, &cpu::ABX, 4 },{ "ROL", &cpu::ROL, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "RTI", &cpu::RTI, &cpu::IMP, 6 },{ "EOR", &cpu::EOR, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 3 },{ "EOR", &cpu::EOR, &cpu::ZP0, 3 },{ "LSR", &cpu::LSR, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PHA", &cpu::PHA, &cpu::IMP, 3 },{ "EOR", &cpu::EOR, &cpu::IMM, 2 },{ "LSR", &cpu::LSR, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "JMP", &cpu::JMP, &cpu::ABS, 3 },{ "EOR", &cpu::EOR, &cpu::ABS, 4 },{ "LSR", &cpu::LSR, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BVC", &cpu::BVC, &cpu::REL, 2 },{ "EOR", &cpu::EOR, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "EOR", &cpu::EOR, &cpu::ZPX, 4 },{ "LSR", &cpu::LSR, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "CLI", &cpu::CLI, &cpu::IMP, 2 },{ "EOR", &cpu::EOR, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "EOR", &cpu::EOR, &cpu::ABX, 4 },{ "LSR", &cpu::LSR, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "RTS", &cpu::RTS, &cpu::IMP, 6 },{ "ADC", &cpu::ADC, &cpu::IZX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 3 },{ "ADC", &cpu::ADC, &cpu::ZP0, 3 },{ "ROR", &cpu::ROR, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "PLA", &cpu::PLA, &cpu::IMP, 4 },{ "ADC", &cpu::ADC, &cpu::IMM, 2 },{ "ROR", &cpu::ROR, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "JMP", &cpu::JMP, &cpu::IND, 5 },{ "ADC", &cpu::ADC, &cpu::ABS, 4 },{ "ROR", &cpu::ROR, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BVS", &cpu::BVS, &cpu::REL, 2 },{ "ADC", &cpu::ADC, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ADC", &cpu::ADC, &cpu::ZPX, 4 },{ "ROR", &cpu::ROR, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "SEI", &cpu::SEI, &cpu::IMP, 2 },{ "ADC", &cpu::ADC, &cpu::ABY, 4 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "ADC", &cpu::ADC, &cpu::ABX, 4 },{ "ROR", &cpu::ROR, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "STA", &cpu::STA, &cpu::IZX, 6 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "STY", &cpu::STY, &cpu::ZP0, 3 },{ "STA", &cpu::STA, &cpu::ZP0, 3 },{ "STX", &cpu::STX, &cpu::ZP0, 3 },{ "???", &cpu::XXX, &cpu::IMP, 3 },{ "DEY", &cpu::DEY, &cpu::IMP, 2 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "TXA", &cpu::TXA, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "STY", &cpu::STY, &cpu::ABS, 4 },{ "STA", &cpu::STA, &cpu::ABS, 4 },{ "STX", &cpu::STX, &cpu::ABS, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },
		{ "BCC", &cpu::BCC, &cpu::REL, 2 },{ "STA", &cpu::STA, &cpu::IZY, 6 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "STY", &cpu::STY, &cpu::ZPX, 4 },{ "STA", &cpu::STA, &cpu::ZPX, 4 },{ "STX", &cpu::STX, &cpu::ZPY, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },{ "TYA", &cpu::TYA, &cpu::IMP, 2 },{ "STA", &cpu::STA, &cpu::ABY, 5 },{ "TXS", &cpu::TXS, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "???", &cpu::NOP, &cpu::IMP, 5 },{ "STA", &cpu::STA, &cpu::ABX, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },
		{ "LDY", &cpu::LDY, &cpu::IMM, 2 },{ "LDA", &cpu::LDA, &cpu::IZX, 6 },{ "LDX", &cpu::LDX, &cpu::IMM, 2 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "LDY", &cpu::LDY, &cpu::ZP0, 3 },{ "LDA", &cpu::LDA, &cpu::ZP0, 3 },{ "LDX", &cpu::LDX, &cpu::ZP0, 3 },{ "???", &cpu::XXX, &cpu::IMP, 3 },{ "TAY", &cpu::TAY, &cpu::IMP, 2 },{ "LDA", &cpu::LDA, &cpu::IMM, 2 },{ "TAX", &cpu::TAX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "LDY", &cpu::LDY, &cpu::ABS, 4 },{ "LDA", &cpu::LDA, &cpu::ABS, 4 },{ "LDX", &cpu::LDX, &cpu::ABS, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },
		{ "BCS", &cpu::BCS, &cpu::REL, 2 },{ "LDA", &cpu::LDA, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "LDY", &cpu::LDY, &cpu::ZPX, 4 },{ "LDA", &cpu::LDA, &cpu::ZPX, 4 },{ "LDX", &cpu::LDX, &cpu::ZPY, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },{ "CLV", &cpu::CLV, &cpu::IMP, 2 },{ "LDA", &cpu::LDA, &cpu::ABY, 4 },{ "TSX", &cpu::TSX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 4 },{ "LDY", &cpu::LDY, &cpu::ABX, 4 },{ "LDA", &cpu::LDA, &cpu::ABX, 4 },{ "LDX", &cpu::LDX, &cpu::ABY, 4 },{ "???", &cpu::XXX, &cpu::IMP, 4 },
		{ "CPY", &cpu::CPY, &cpu::IMM, 2 },{ "CMP", &cpu::CMP, &cpu::IZX, 6 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "CPY", &cpu::CPY, &cpu::ZP0, 3 },{ "CMP", &cpu::CMP, &cpu::ZP0, 3 },{ "DEC", &cpu::DEC, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "INY", &cpu::INY, &cpu::IMP, 2 },{ "CMP", &cpu::CMP, &cpu::IMM, 2 },{ "DEX", &cpu::DEX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "CPY", &cpu::CPY, &cpu::ABS, 4 },{ "CMP", &cpu::CMP, &cpu::ABS, 4 },{ "DEC", &cpu::DEC, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BNE", &cpu::BNE, &cpu::REL, 2 },{ "CMP", &cpu::CMP, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "CMP", &cpu::CMP, &cpu::ZPX, 4 },{ "DEC", &cpu::DEC, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "CLD", &cpu::CLD, &cpu::IMP, 2 },{ "CMP", &cpu::CMP, &cpu::ABY, 4 },{ "NOP", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "CMP", &cpu::CMP, &cpu::ABX, 4 },{ "DEC", &cpu::DEC, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
		{ "CPX", &cpu::CPX, &cpu::IMM, 2 },{ "SBC", &cpu::SBC, &cpu::IZX, 6 },{ "???", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "CPX", &cpu::CPX, &cpu::ZP0, 3 },{ "SBC", &cpu::SBC, &cpu::ZP0, 3 },{ "INC", &cpu::INC, &cpu::ZP0, 5 },{ "???", &cpu::XXX, &cpu::IMP, 5 },{ "INX", &cpu::INX, &cpu::IMP, 2 },{ "SBC", &cpu::SBC, &cpu::IMM, 2 },{ "NOP", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::SBC, &cpu::IMP, 2 },{ "CPX", &cpu::CPX, &cpu::ABS, 4 },{ "SBC", &cpu::SBC, &cpu::ABS, 4 },{ "INC", &cpu::INC, &cpu::ABS, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },
		{ "BEQ", &cpu::BEQ, &cpu::REL, 2 },{ "SBC", &cpu::SBC, &cpu::IZY, 5 },{ "???", &cpu::XXX, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 8 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "SBC", &cpu::SBC, &cpu::ZPX, 4 },{ "INC", &cpu::INC, &cpu::ZPX, 6 },{ "???", &cpu::XXX, &cpu::IMP, 6 },{ "SED", &cpu::SED, &cpu::IMP, 2 },{ "SBC", &cpu::SBC, &cpu::ABY, 4 },{ "NOP", &cpu::NOP, &cpu::IMP, 2 },{ "???", &cpu::XXX, &cpu::IMP, 7 },{ "???", &cpu::NOP, &cpu::IMP, 4 },{ "SBC", &cpu::SBC, &cpu::ABX, 4 },{ "INC", &cpu::INC, &cpu::ABX, 7 },{ "???", &cpu::XXX, &cpu::IMP, 7 },
	};

}

CPU6502::~CPU6502()
{
}

uint8_t CPU6502::read(uint16_t addr)
{
  return this->bus->read(addr, false);
}

void CPU6502::write(uint16_t addr, uint8_t val)
{
  this->bus->write(addr,val);
}

void CPU6502::clock(){
  if(this->cycles == 0){
    this->opcode = this->read(this->pc);
    this->pc++;

    // initial number of cycles
    this->cycles = this->lookup[this->opcode].cycles;

    uint8_t additional_cycle1 = (this->*lookup[this->opcode].addrmode)();
    uint8_t additional_cycle2  =(this->*lookup[this->opcode].operate )();
    this->cycles += (additional_cycle1 & additional_cycle2);

  }
  this->cycles--;
}

// FLAG FUNCTIONS

// Returns the value of a specific bit of the status register
uint8_t CPU6502::getFlag(FLAGS6502 f)
{
	return ((status & f) > 0) ? 1 : 0;
}

void CPU6502::setFlag(CPU6502::FLAGS6502 f, bool v){
  if(v){
    this->status |= f;
  }else{
    this->status &= ~f;
  }
}




// This function sources the data used by the instruction into 
// a convenient numeric variable. Some instructions dont have to 
// fetch data as the source is implied by the instruction. For example
// "INX" increments the X register. There is no additional data
// required. For all other addressing modes, the data resides at 
// the location held within addr_abs, so it is read from there. 
// Immediate adress mode exploits this slightly, as that has
// set addr_abs = pc + 1, so it fetches the data from the
// next byte for example "LDA $FF" just loads the accumulator with
// 256, i.e. no far reaching memory fetch is required. "fetched"
// is a variable global to the CPU, and is set by calling this 
// function. It also returns it for convenience.
uint8_t CPU6502::fetch(){
  if(!(this->lookup[this->opcode].addrmode == &CPU6502::IMP)){
    this->fetched = this->read(this->addr_abs);
  }
  return this->fetched;
}







// Forces the 6502 into a known state. This is hard-wired inside the CPU. The
// registers are set to 0x00, the status register is cleared except for unused
// bit which remains at 1. An absolute address is read from location 0xFFFC
// which contains a second address that the program counter is set to. This 
// allows the programmer to jump to a known and programmable location in the
// memory to start executing from. Typically the programmer would set the value
// at location 0xFFFC at compile time.
void CPU6502::reset()
{
	// Get address to set program counter to
	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);

	// Set it
	pc = (hi << 8) | lo;

	// Reset internal registers
	a = 0;
	x = 0;
	y = 0;
	stkpt = 0xFF;
	status = 0x00 | U;

	// Clear internal helper variables
	addr_rel = 0x0000;
	addr_abs = 0x0000;
	fetched = 0x00;

	// Reset takes time
	cycles = 8;
}

// Interrupt requests are a complex operation and only happen if the
// "disable interrupt" flag is 0. IRQs can happen at any time, but
// you dont want them to be destructive to the operation of the running 
// program. Therefore the current instruction is allowed to finish
// (which I facilitate by doing the whole thing when cycles == 0) and 
// then the current program counter is stored on the stack. Then the
// current status register is stored on the stack. When the routine
// that services the interrupt has finished, the status register
// and program counter can be restored to how they where before it 
// occurred. This is impemented by the "RTI" instruction. Once the IRQ
// has happened, in a similar way to a reset, a programmable address
// is read form hard coded location 0xFFFE, which is subsequently
// set to the program counter.
void CPU6502::irq()
{
	// If interrupts are allowed
	if (getFlag(I) == 0)
	{
		// Push the program counter to the stack. It's 16-bits dont
		// forget so that takes two pushes

		uint16_t hi = (pc >> 8)  & 0x00FF;
		write(0x0100 + stkpt,hi );
		stkpt--;

		uint16_t lo = pc & 0x00FF;
		write(0x0100 + stkpt, lo);
		stkpt--;

		// Then Push the status register to the stack
		setFlag(B, 0);
		setFlag(U, 1);
		setFlag(I, 1);
		write(0x0100 + stkpt, status);
		stkpt--;

		// Read new program counter location from fixed address
		addr_abs = 0xFFFE;
		lo = read(addr_abs + 0);
		hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		// IRQs take time
		cycles = 7;
	}
}

// A Non-Maskable Interrupt cannot be ignored. It behaves in exactly the
// same way as a regular IRQ, but reads the new program counter address
// form location 0xFFFA.
void CPU6502::nmi()
{
	write(0x0100 + stkpt, (pc >> 8) & 0x00FF);
	stkpt--;
	write(0x0100 + stkpt, pc & 0x00FF);
	stkpt--;

	setFlag(B, 0);
	setFlag(U, 1);
	setFlag(I, 1);
	write(0x0100 + stkpt, status);
	stkpt--;

	addr_abs = 0xFFFA;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	pc = (hi << 8) | lo;

	cycles = 8;
}


///////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS

bool CPU6502::complete()
{
	return cycles == 0;
}

// This is the disassembly function. Its workings are not required for emulation.
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> CPU6502::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with 
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = bus->read(addr, true); addr++;
		sInst += lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (lookup[opcode].addrmode == &CPU6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::IMM)
		{
			value = bus->read(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::ZP0)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;												
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::ZPX)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::ZPY)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::IZX)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::IZY)
		{
			lo = bus->read(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::ABS)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::ABX)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::ABY)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::IND)
		{
			lo = bus->read(addr, true); addr++;
			hi = bus->read(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup[opcode].addrmode == &CPU6502::REL)
		{
			value = bus->read(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}
