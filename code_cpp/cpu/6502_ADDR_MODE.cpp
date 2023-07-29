#include "6502.h"

// Addressing Modes =============================================

// Address Mode: Implied
// There is no additional data required for this instruction. The instruction
// does something very simple like like sets a status bit. However, we will
// target the accumulator, for instructions like PHA
uint8_t CPU6502::IMP()
{
  this->fetched = this->a;
  return 0;
}

// Address Mode: Immediate
// The instruction expects the next byte to be used as a value, so we'll prep
// the read address to point to the next byte
uint8_t CPU6502::IMM()
{
  this->addr_abs = this->pc++;
  return 0;
}

// Address Mode: Zero Page
// To save program bytes, zero page addressing allows you to absolutely address
// a location in first 0xFF bytes of address range. Clearly this only requires
// one byte instead of the usual two.
uint8_t CPU6502::ZP0()
{

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
uint8_t CPU6502::ZPX()
{
  // Read the next byte from the Program Counter (PC).
  uint8_t offset = this->read(this->pc);

  // Add the value of the X register to the offset to get the absolute address.
  uint16_t addr_abs = offset + this->x;

  // Increment the Program Counter (PC) to point to the next instruction.
  this->pc++;

  // Define a bitmask to ensure that the absolute address is within the valid range (0x00 to 0xFF).
  const uint16_t ADDRESS_MASK = 0x00FF;

  // Ensure that the absolute address is within the valid range (0x00 to 0xFF).
  addr_abs = addr_abs & ADDRESS_MASK;

  // Return 0 to indicate success.
  return 0;
}

// Address Mode: Zero Page with Y Offset
// Same as above but uses Y Register for offset
uint8_t CPU6502::ZPY()
{
  // Read the next byte from the Program Counter (PC).
  uint8_t offset = this->read(this->pc);

  // Add the value of the Y register to the offset to get the absolute address.
  uint16_t addr_abs = offset + this->y;

  // Define a bitmask to ensure that the absolute address is within the valid range (0x00 to 0xFF).
  const uint16_t ADDRESS_MASK = 0x00FF;

  // Apply the bitmask to the absolute address.
  addr_abs &= ADDRESS_MASK;

  // Increment the Program Counter (PC) to point to the next instruction.
  this->pc++;

  // Return 0 to indicate success.
  return 0;
}

// Address Mode: Absolute
// A full 16-bit address is loaded and used
uint8_t CPU6502::ABS()
{
  // Read the low byte of the absolute memory address from the Program Counter (PC).
  uint8_t lowByte = this->read(this->pc);

  // Increment the Program Counter (PC) to point to the next byte (high byte).
  this->pc++;

  // Read the high byte of the absolute memory address from the updated PC.
  uint8_t highByte = this->read(this->pc);

  // Increment the Program Counter (PC) again to point to the next instruction.
  this->pc++;

  // Combine the low and high bytes to form the 16-bit absolute memory address.
  this->addr_abs = static_cast<uint16_t>(lowByte) | (static_cast<uint16_t>(highByte) << 8);

  // Return 0 to indicate success.
  return 0;
}

// Address Mode: Absolute with X Offset
// Fundamentally the same as absolute addressing, but the contents of the X Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t CPU6502::ABX()
{

  // ex:
  // lowByte = 0xA2;
  // highByte = 0x01;
  // highByte = hi<<8 = 0x0100;
  // addr = highByte | lowByte;
  // addr = 0x0100 | 0x00A2 = 0x01A2

  // Read the low byte of the absolute memory address from the Program Counter (PC).
  uint8_t lowByte = this->read(this->pc);
  this->pc++;

  // Read the high byte of the absolute memory address from the updated PC.
  uint8_t highByte = this->read(this->pc);
  this->pc++;

  // Combine the low and high bytes to form the 16-bit absolute memory address.
  uint16_t addr_abs_hi_shifted = static_cast<uint16_t>(highByte) << 8;
  uint16_t addr_abs = addr_abs_hi_shifted | lowByte;

  // Add the value of the X register to the absolute memory address.
  this->addr_abs += this->x;

  // Separate the part to check for a page boundary crossing into a variable.
  uint16_t addr_abs_upper_byte = addr_abs & 0xFF00;

  // Check if a page boundary crossing occurs by comparing the high bytes before and after adding X.
  bool pageBoundaryCrossed = addr_abs_upper_byte != addr_abs_hi_shifted;

  // Return 1 if a page boundary crossing occurred, 0 otherwise.
  return pageBoundaryCrossed ? 1 : 0;
}

// Address Mode: Absolute with Y Offset
// Fundamentally the same as absolute addressing, but the contents of the Y Register
// is added to the supplied two byte address. If the resulting address changes
// the page, an additional clock cycle is required
uint8_t CPU6502::ABY()
{
  // Read the low byte of the absolute memory address from the Program Counter (PC).
  uint8_t lowByte = this->read(this->pc);
  this->pc++;

  // Read the high byte of the absolute memory address from the updated PC.
  uint8_t highByte = this->read(this->pc);
  this->pc++;

  // Combine the low and high bytes to form the 16-bit absolute memory address.
  uint16_t addr_abs_hi_shifted = static_cast<uint16_t>(highByte) << 8;
  uint16_t addr_abs = addr_abs_hi_shifted | lowByte;

  // Add the value of the Y register to the absolute memory address.
  uint16_t addr_abs_with_y_offset = addr_abs + this->y;

  // Check if a page boundary crossing occurs by comparing the high bytes before and after adding Y.
  uint16_t addr_abs_upper_byte = addr_abs_with_y_offset & 0xFF00;
  bool pageBoundaryCrossed = addr_abs_upper_byte != addr_abs_hi_shifted;

  // Return 1 if a page boundary crossing occurred, 0 otherwise.
  return pageBoundaryCrossed ? 1 : 0;
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
uint8_t CPU6502::IND()
{
  // Read the low byte of the indirect memory address from the Program Counter (PC).
  uint8_t ptr_lo = this->read(this->pc);
  this->pc++;

  // Read the high byte of the indirect memory address from the updated PC.
  uint8_t ptr_hi = this->read(this->pc);
  this->pc++;

  // Combine the low and high bytes to form the 16-bit indirect memory address pointer.
  uint16_t ptr = (static_cast<uint16_t>(ptr_hi) << 8) | ptr_lo;

  uint8_t addr_lo, addr_hi;

  if (ptr_lo == 0x00FF)
  {
    // Simulate page boundary hardware bug when the low byte is 0x00FF.
    // In this case, we read the low byte from the given address and the high byte from the same page.
    addr_hi = this->read(ptr & 0xFF00) << 8;
    addr_lo = this->read(ptr);
  }
  else
  {
    // Behave normally.
    // Read the low byte from the address pointed by ptr and the high byte from the next address.
    addr_hi = this->read(ptr + 1);
    addr_lo = this->read(ptr);
  }

  // Combine the low and high bytes to form the final 16-bit address.
  this->addr_abs = (static_cast<uint16_t>(addr_hi) << 8) | addr_lo;

  return 0;
}

// Address Mode: Indirect X
// The supplied 8-bit address is offset by X Register to index
// a location in page 0x00. The actual 16-bit address is read
// from this location
uint8_t CPU6502::IZX()
{
  // Read the zero-page address from the Program Counter (PC).
  uint16_t base_addr = this->read(this->pc);
  this->pc++;

  // Calculate the effective address using the X register.
  uint16_t indexed_addr = base_addr + static_cast<uint16_t>(this->x);

  // Wrap the indexed address within the zero-page address range (0x0000 to 0x00FF).
  indexed_addr &= 0x00FF;

  // Read the low and high bytes from the indexed address.
  uint16_t lo_byte = this->read(indexed_addr);
  uint16_t hi_byte = this->read(indexed_addr + 1);

  // Combine the low and high bytes to form the final 16-bit address.
  this->addr_abs = (hi_byte << 8) | lo_byte;

  // Always return 0.
  return 0;
}

// Address Mode: Indirect Y
// The supplied 8-bit address indexes a location in page 0x00. From
// here the actual 16-bit address is read, and the contents of
// Y Register is added to it to offset it. If the offset causes a
// change in page then an additional clock cycle is required.
uint8_t CPU6502::IZY()
{
  // Read the zero-page address from the Program Counter (PC).
  uint16_t base_addr = this->read(this->pc);
  this->pc++;

  // Read the low byte from the zero-page address.
  uint16_t lo_byte = this->read(base_addr & 0x00FF);

  // Read the high byte from the next address after the zero-page address.
  uint16_t hi_byte = this->read((base_addr + 1) & 0x00FF);

  // Combine the low and high bytes to form the initial 16-bit address.
  uint16_t addr = (hi_byte << 8) | lo_byte;

  // Add the Y register to the initial address to get the final address.
  this->addr_abs = addr + static_cast<uint16_t>(this->y);

  // Check for page crossing by comparing the high bytes before and after adding Y.
  if ((this->addr_abs & 0xFF00) != (hi_byte << 8))
  {
    return 1;
  }

  // Always return 0.
  return 0;
}

// Address Mode: Relative
// This address mode is exclusive to branch instructions. The address
// must reside within -128 to +127 of the branch instruction, i.e.
// you cant directly branch to any address in the addressable range.
uint8_t CPU6502::REL()
{
  // Read the relative offset from the Program Counter (PC).
  this->addr_rel = this->read(this->pc);
  this->pc++;

  // Check if the relative offset is negative (bit 7 is set).
  // If it is, sign-extend the offset to ensure it's treated as a negative value in 16-bit signed arithmetic.
  if (this->addr_rel & 0x80)
  {
    this->addr_rel |= 0xFF00;
  }

  // Always return 0.
  return 0;
}