#include "Mapper_0.h"
#include <iostream>
Mapper_0::Mapper_0(uint8_t prg_banks, uint8_t chr_banks) : Mapper(prg_banks, chr_banks) {}

Mapper_0::~Mapper_0() {}

bool Mapper_0::cpuMapRead(uint16_t addr, uint32_t &mapped_addr)
{
  // if PRGROM is 16KB
  //     CPU Address Bus          PRG ROM
  //     0x8000 -> 0xBFFF: Map    0x0000 -> 0x3FFF
  //     0xC000 -> 0xFFFF: Mirror 0x0000 -> 0x3FFF
  // if PRGROM is 32KB
  //     CPU Address Bus          PRG ROM
  //     0x8000 -> 0xFFFF: Map    0x0000 -> 0x7FFF
  if (addr >= PRG_ROM_START && addr <= PRG_ROM_END)
  {
    const uint16_t PRG_BANK_MASK = (this->prg_banks > 1) ? 0x7FFF : 0x3FFF;
    mapped_addr = addr & PRG_BANK_MASK;
    return true;
  }

  return false;
}
bool Mapper_0::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr)
{

  if (addr >= PRG_ROM_START && addr < PRG_ROM_END)
  {
    const uint16_t PRG_BANK_MASK = (this->prg_banks > 1) ? 0x7FFF : 0x3FFF;
    mapped_addr = addr & PRG_BANK_MASK;
    return true;
  }
  return false;
}
bool Mapper_0::ppuMapRead(uint16_t addr, uint32_t &mapped_addr)
{
  // There is no mapping required for PPU
  // PPU Address Bus          CHR ROM
  // 0x0000 -> 0x1FFF: Map    0x0000 -> 0x1FFF
  if (addr >= PPU_RAM_START && addr <= PPU_RAM_END)
  {
    mapped_addr = addr;
    return true;
  }
  return false;
}
bool Mapper_0::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr)
{
  // if (addr >= PPU_RAM_START && addr <= PPU_RAM_END)
  // {

  //   return true;
  // }
  return false;
}