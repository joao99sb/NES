#include "6502.h"
#include "Bus.h"

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

// Address Mode: Implied
// There is no additional data required for this instruction. The instruction
// does something very simple like like sets a status bit. However, we will
// target the accumulator, for instructions like PHA
uint8_t CPU6502::IMP(){
  this->fetched = this->a;
  return 0;
}

// Address Mode: Immediate
// The instruction expects the next byte to be used as a value, so we'll prep
// the read address to point to the next byte
uint8_t CPU6502::IMM(){
  this->addr_abs = this->pc++;
  return 0;
}


// Address Mode: Zero Page
// To save program bytes, zero page addressing allows you to absolutely address
// a location in first 0xFF bytes of address range. Clearly this only requires
// one byte instead of the usual two.
uint8_t CPU6502::ZP0(){

  // Esse modo de endereçamento permite acessar
  // diretamente a memória localizada nas primeiras 256 posições (0x0000 a 0x00FF) da memória RAM,
  // também conhecida como "página zero".
  
  this->addr_abs = this->read(this->pc);
  this->pc++;
  // self and bitwise operator
  this->addr_abs &= 0x00FF;

  return 0;
}

// Address Mode: Zero Page with X Offset
// Fundamentally the same as Zero Page addressing, but the contents of the X Register
// is added to the supplied single byte address. This is useful for iterating through
// ranges within the first page.
uint8_t CPU6502::ZPX(){
  this->addr_abs = this->read(this->pc) + this->x;
  this->pc++;
  this->addr_abs &= 0x00FF;
  return 0;

}

// Address Mode: Zero Page with Y Offset
// Same as above but uses Y Register for offset
uint8_t CPU6502::ZPY(){
  this->addr_abs = this->read(this->pc) + this->y;
  this->pc++;
  this->addr_abs &= 0x00FF;
  return 0;
}

// Address Mode: Absolute 
// A full 16-bit address is loaded and used
uint8_t CPU6502::ABS(){
  this->addr_abs = this->read(this->pc);
  this->pc++;
  this->addr_abs |= this->read(this->pc) << 8;
  this->pc++;
  return 0;
}

// Address Mode: Absolute with X Offset
// Fundamentally the same as absolute addressing, but the contents of the X Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t CPU6502::ABX(){

  // ex:
  // lo = 0xA2;
  // hi = 0x01;
  // hi = hi<<8 = 0x0100;
  // addr = hi | lo; 
  // addr = 0x0100 | 0x00A2 = 0x01A2



  uint8_t lo = this->read(this->pc);
  this->pc++;
  uint8_t hi = this->read(this->pc);
  this->pc++;
  this->addr_abs = (hi << 8) | lo;
  this->addr_abs+= this->x;
  
  if((addr_abs & 0xFF00) != (hi<< 8)) return 1;
  return 0;

}


// Address Mode: Absolute with Y Offset
// Fundamentally the same as absolute addressing, but the contents of the Y Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t CPU6502::ABY(){
  uint8_t lo = this->read(this->pc);
  this->pc++;
  uint8_t hi = this->read(this->pc);
  this->pc++;
  this->addr_abs = (hi << 8) | lo;
  this->addr_abs+= this->y;

  if((addr_abs & 0xFF00)!= (hi<< 8)) return 1;
  return 0;
}

// Note: The next 3 address modes use indirection (aka Pointers!)

// Address Mode: Indirect
// The supplied 16-bit address is read to get the actual 16-bit address. This is
// instruction is unusual in that it has a bug in the hardware! To emulate its
// function accurately, we also need to emulate this bug. If the low byte of the
// supplied address is 0xFF, then to read the high byte of the actual address
// we need to cross a page boundary. This doesnt actually work on the chip as 
// designed, instead it wraps back around in the same page, yielding an 
// invalid actual address
uint8_t CPU6502::IND(){
  uint16_t ptr_lo = this->read(this->pc);
  this->pc++;
  uint16_t ptr_hi = this->read(this->pc);
  this->pc++;

  uint16_t ptr = (ptr_hi << 8 ) | ptr_lo;

  if(ptr_lo == 0x00FF){ // simulate page boundary hardware bug
    this->addr_abs = (this->read(ptr & 0xff00)<< 8 )| this->read(ptr+0);
  }else{ // behave normally
    this->addr_abs = (this->read(ptr+1)<< 8 )| this->read(ptr+1);
  }

  return 0;
}

// Address Mode: Indirect X
// The supplied 8-bit address is offset by X Register to index
// a location in page 0x00. The actual 16-bit address is read 
// from this location
uint8_t CPU6502::IZX(){
  uint16_t addr = this->read(this->pc);
  this->pc++;

  uint16_t lo = this->read((uint16_t)(addr + (uint16_t)this->x) & 0x00ff);
  uint16_t hi = this->read((uint16_t)(addr + (uint16_t)this->x + 1) & 0x00ff);

  this->addr_abs = (hi << 8) | lo;
  return 0;
}

// Address Mode: Indirect Y
// The supplied 8-bit address indexes a location in page 0x00. From 
// here the actual 16-bit address is read, and the contents of
// Y Register is added to it to offset it. If the offset causes a
// change in page then an additional clock cycle is required.
uint8_t CPU6502::IZY(){

  uint16_t addr = this->read(this->pc);
  this->pc++;

  uint16_t lo = this->read(addr & 0x00ff);
  uint16_t hi = this->read((addr + 1) & 0x00ff);

  this->addr_abs = (hi << 8) | lo;
  this->addr_abs += this->y;
  
  if((this->addr_abs & 0xff00) != (hi << 8)){
    return 1;
  }

  return 0;
}

// Address Mode: Relative
// This address mode is exclusive to branch instructions. The address
// must reside within -128 to +127 of the branch instruction, i.e.
// you cant directly branch to any address in the addressable range.
uint8_t CPU6502::REL(){
  this->addr_rel = this->read(this->pc);
  this->pc++;
  if(this->addr_rel & 0x80) this->addr_rel |= 0xff00;
  
  return 0;
}

// instructions 

uint8_t CPU6502::fetch(){
  if(!(this->lookup[this->opcode].addrmode == &CPU6502::IMP)){
    this->fetched = this->read(this->addr_abs);
  }
  return this->fetched;
}

uint8_t CPU6502::AND(){
  this->fetch();
  this->a &= this->fetched;
  this->setFlag(this->Z,this->a==0x00);
  this->setFlag(this->N,this->a==0x80);

  return 1; 

}

uint8_t CPU6502::BCS(){

  if(this->getFlag(C) == 1){
    this->cycles++;
    this->addr_abs = this->pc + this->addr_rel;

    if((this->addr_abs & 0xff00) != (this->pc & 0xff00) ){
      this->cycles++;
    }

    this->pc = this->addr_abs;
  }

  return 0;

}
uint8_t CPU6502::BCC(){

  if(this->getFlag(C) == 0){
    this->cycles++;
    this->addr_abs = this->pc + this->addr_rel;

    if((this->addr_abs & 0xff00) != (this->pc & 0xff00) ){
      this->cycles++;
    }

    this->pc = this->addr_abs;
  }

  return 0;

}
uint8_t CPU6502::BEQ(){

	if (this->getFlag(Z) == 1)
	{
		this->cycles++;
		this->addr_abs = pc + addr_rel;

		if ((this->addr_abs & 0xFF00) != (this->pc & 0xFF00))
			cycles++;

		this->pc = this->addr_abs;
	}
	return 0;

}
uint8_t CPU6502::BMI()
{
	if (this->getFlag(N) == 1)
	{
	this->cycles++;
		this->addr_abs = pc + addr_rel;

		if ((this->addr_abs & 0xFF00) != (this->pc & 0xFF00))
			cycles++;

		this->pc = this->addr_abs;
	}
	return 0;
}

	
uint8_t CPU6502::BPL(){
  	if (this->getFlag(N) == 0)
	{
	this->cycles++;
		this->addr_abs = pc + addr_rel;

		if ((this->addr_abs & 0xFF00) != (this->pc & 0xFF00))
			cycles++;

		this->pc = this->addr_abs;
	}
	return 0;
}
uint8_t CPU6502::BVC(){
  	if (this->getFlag(V) == 0)
	{
	this->cycles++;
		this->addr_abs = pc + addr_rel;

		if ((this->addr_abs & 0xFF00) != (this->pc & 0xFF00))
			cycles++;

		this->pc = this->addr_abs;
	}
	return 0;
}
uint8_t CPU6502::BVS(){
  	if (this->getFlag(V) == 1)
	{
	this->cycles++;
		this->addr_abs = pc + addr_rel;

		if ((this->addr_abs & 0xFF00) != (this->pc & 0xFF00))
			cycles++;

		this->pc = this->addr_abs;
	}
	return 0;
}
uint8_t CPU6502::CLC(){

  this->setFlag(C,false);
  return 0;

}
uint8_t CPU6502::CLD(){

  this->setFlag(D,false);
  return 0;

}

uint8_t CPU6502::ADC(){
  this -> fetch();
  uint16_t temp = (uint16_t)a + (uint16_t)fetched + getFlag(C);
  setFlag(C,temp > 255);
  setFlag(Z ,(temp &  0x00ff)==0);
  setFlag(N,temp &  0x08);
 	// The signed Overflow flag is set based on all that up there! :D
	setFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);
	
  this->a = temp & 0x00ff;
  return 1;

}
uint8_t CPU6502::SBC(){

  fetch();
	
	// Operating in 16-bit domain to capture carry out
	
	// We can invert the bottom 8 bits with bitwise xor
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
	
	// Notice this is exactly the same as addition from here!
	uint16_t temp = (uint16_t)a + value + (uint16_t)getFlag(C);
	setFlag(C, temp & 0xFF00);
	setFlag(Z, ((temp & 0x00FF) == 0));
	setFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
	setFlag(N, temp & 0x0080);
	a = temp & 0x00FF;
	return 1;
}


// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t CPU6502::ASL()
{
	this->fetch();
	uint16_t temp = (uint16_t)fetched << 1;
	setFlag(C, (temp & 0xFF00) > 0);
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, temp & 0x80);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::BIT()
{
	fetch();
	uint16_t temp = a & fetched;
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, fetched & (1 << 7));
	setFlag(V, fetched & (1 << 6));
	return 0;
}

uint8_t CPU6502::BNE()
{
	if (this->getFlag(Z) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t CPU6502::BRK()
{
	pc++;
	
	setFlag(I, 1);
	write(0x0100 + stkpt, (pc >> 8) & 0x00FF);
	stkpt--;
	write(0x0100 + stkpt, pc & 0x00FF);
	stkpt--;

	setFlag(B, 1);
	write(0x0100 + stkpt, status);
	stkpt--;
	setFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}

// Instruction: Disable Interrupts / Clear Interrupt Flag
// Function:    I = 0
uint8_t CPU6502::CLI()
{
	setFlag(I, false);
	return 0;
}

// Instruction: Clear Overflow Flag
// Function:    V = 0
uint8_t CPU6502::CLV()
{
	setFlag(V, false);
	return 0;
}

// Instruction: Compare Accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CMP()
{
	fetch();
	uint16_t temp = (uint16_t)a - (uint16_t)fetched;
	setFlag(C, a >= fetched);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 1;
}


// Instruction: Compare X Register
// Function:    C <- X >= M      Z <- (X - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CPX()
{
	fetch();
	uint16_t temp = (uint16_t)x - (uint16_t)fetched;
	setFlag(C, x >= fetched);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CPY()
{
	fetch();
	uint16_t temp = (uint16_t)y - (uint16_t)fetched;
	setFlag(C, y >= fetched);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement Value at Memory Location
// Function:    M = M - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEC()
{
	fetch();
	uint16_t temp = fetched - 1;
	write(addr_abs, temp & 0x00FF);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEX()
{
	x--;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}

// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEY()
{
	y--;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 0;
}


// Instruction: Bitwise Logic XOR
// Function:    A = A xor M
// Flags Out:   N, Z
uint8_t CPU6502::EOR()
{
	fetch();
	a = a ^ fetched;	
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 1;
}


// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t CPU6502::INC()
{
	fetch();
	uint16_t temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   N, Z
uint8_t CPU6502::INX()
{
	x++;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}


// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   N, Z
uint8_t CPU6502::INY()
{
	y++;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 0;
}


// Instruction: Jump To Location
// Function:    pc = address
uint8_t CPU6502::JMP()
{
	pc = addr_abs;
	return 0;
}


// Instruction: Jump To Sub-Routine
// Function:    Push current pc to stack, pc = address
uint8_t CPU6502::JSR()
{
	pc--;

	write(0x0100 + stkpt, (pc >> 8) & 0x00FF);
	stkpt--;
	write(0x0100 + stkpt, pc & 0x00FF);
	stkpt--;

	pc = addr_abs;
	return 0;
}


// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t CPU6502::LDA()
{
	fetch();
	a = fetched;
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 1;
}


// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t CPU6502::LDX()
{
	fetch();
	x = fetched;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 1;
}


// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t CPU6502::LDY()
{
	fetch();
	y = fetched;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 1;
}

uint8_t CPU6502::LSR()
{
	fetch();
	setFlag(C, fetched & 0x0001);
	uint16_t temp = fetched >> 1;	
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::NOP()
{
	// Sadly not all NOPs are equal, Ive added a few here
	// based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
	// and will add more based on game compatibility, and ultimately
	// I'd like to cover all illegal opcodes too
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t CPU6502::ORA()
{
	fetch();
	a = a | fetched;
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 1;
}


// Instruction: Push Accumulator to Stack
// Function:    A -> stack
uint8_t CPU6502::PHA()
{
	write(0x0100 + stkpt, a);
	stkpt--;
	return 0;
}


// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t CPU6502::PHP()
{
	write(0x0100 + stkpt, status | B | U);
	setFlag(B, 0);
	setFlag(U, 0);
	stkpt--;
	return 0;
}


// Instruction: Pop Accumulator off Stack
// Function:    A <- stack
// Flags Out:   N, Z
uint8_t CPU6502::PLA()
{
	stkpt++;
	a = read(0x0100 + stkpt);
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 0;
}


// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t CPU6502::PLP()
{
	stkpt++;
	status = read(0x0100 + stkpt);
	setFlag(U, 1);
	return 0;
}

uint8_t CPU6502::ROL()
{
	fetch();
	uint16_t temp = (uint16_t)(fetched << 1) | getFlag(C);
	setFlag(C, temp & 0xFF00);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::ROR()
{
	fetch();
	uint16_t temp = (uint16_t)(getFlag(C) << 7) | (fetched >> 1);
	setFlag(C, fetched & 0x01);
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::RTI()
{
	stkpt++;
	status = read(0x0100 + stkpt);
	status &= ~B;
	status &= ~U;

	stkpt++;
	pc = (uint16_t)read(0x0100 + stkpt);
	stkpt++;
	pc |= (uint16_t)read(0x0100 + stkpt) << 8;
	return 0;
}

uint8_t CPU6502::RTS()
{
	stkpt++;
	pc = (uint16_t)read(0x0100 + stkpt);
	stkpt++;
	pc |= (uint16_t)read(0x0100 + stkpt) << 8;
	
	pc++;
	return 0;
}




// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t CPU6502::SEC()
{
	setFlag(C, true);
	return 0;
}


// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t CPU6502::SED()
{
	setFlag(D, true);
	return 0;
}


// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t CPU6502::SEI()
{
	setFlag(I, true);
	return 0;
}


// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t CPU6502::STA()
{
	write(addr_abs, a);
	return 0;
}


// Instruction: Store X Register at Address
// Function:    M = X
uint8_t CPU6502::STX()
{
	write(addr_abs, x);
	return 0;
}


// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t CPU6502::STY()
{
	write(addr_abs, y);
	return 0;
}


// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t CPU6502::TAX()
{
	x = a;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}


// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t CPU6502::TAY()
{
	y = a;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 0;
}


// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t CPU6502::TSX()
{
	x = stkpt;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t CPU6502::TXA()
{
	a = x;
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t CPU6502::TXS()
{
	stkpt = x;
	return 0;
}


// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t CPU6502::TYA()
{
	a = y;
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 0;
}


// This function captures illegal opcodes
uint8_t CPU6502::XXX()
{
	return 0;
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
