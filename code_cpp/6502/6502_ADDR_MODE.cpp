#include "6502.h"


// Addressing Modes =============================================


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