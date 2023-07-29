#pragma once
#include "Mapper.h"

class Mapper_0 : public Mapper
{
public:
  Mapper_0(uint8_t prg_banks, uint8_t chr_banks);
  ~Mapper_0();

  virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
  virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
  virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
  virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
};