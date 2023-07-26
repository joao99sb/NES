#include "Bus.h"

Bus::Bus()
{

  // clear RAM contents, just in case
  for (auto &i : this->ram)
  {
    i = 0x00;
  }

  // connect CPU to  communication bus
  cpu.ConnectBus(this);
}

Bus::~Bus()
{
}

uint8_t Bus::read(uint16_t addr, bool read_only)
{
  if (addr >= 0x0000 && addr <= 0xffff)
  {
    return this->ram[addr];
  }
  return 0x00;
}

void Bus::write(uint16_t addr, uint8_t data)
{

  if (addr >= 0x0000 && addr <= 0xffff)
  {
    this->ram[addr] = data;
  }
}
