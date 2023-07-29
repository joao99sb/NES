#include "Mapper.h"



Mapper::Mapper(uint8_t prg_banks, uint8_t chr_banks)
{
  this->prg_banks = prg_banks;
  this->chr_banks = chr_banks;
}

Mapper::~Mapper()
{
}
