#pragma once

#include <cstdint>
#include "../Constants.h"

class Mapper
{
private:
  /* data */
public:
  Mapper(uint8_t prg_banks, uint8_t chr_banks);
  ~Mapper();

  virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
  virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;
  virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
  virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

protected:
  uint8_t prg_banks = 0;
  uint8_t chr_banks = 0;
};
