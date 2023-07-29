#include "Bus.h"

Bus::Bus()
{

  // clear cpuRam contents, just in case
  for (auto &i : this->cpuRam)
  {
    i = 0x00;
  }

  // connect CPU to  communication bus
  cpu.ConnectBus(this);
}

Bus::~Bus()
{
}

uint8_t Bus::cpuRead(uint16_t addr, bool read_only)
{

  uint8_t data = 0x00;
  if (this->cart->cpuRead(addr, data))
  {
    // Catridge Addr range
  }

  else if (addr >= 0x0000 && addr <= 0x1fff) // da uma olhada no endereco!
  {

    // addr = 0x0801     0b1000_0000_0001
    //        &       =  &
    // mask = 0x07FF     0b0111_1111_1111
    //                     0000_0000_0001

    // System RAM Address Range. The range covers 8KB, though
    // there is only 2KB available. That 2KB is "mirrored"
    // through this address range. Using bitwise AND to mask
    // the bottom 11 bits is the same as addr % 2048.

    data = this->cpuRam[addr & CPU_RAM_MASK];
  }
  else if (addr >= 0x2000 && addr <= 0x3fff)
  {
    data = this->ppu.cpuRead(addr && 0x0007, read_only);
  }
  return data;
}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{

  if (this->cart->cpuWrite(addr, data))
  {
    // Catridge Addr range
  }

  else if (addr >= 0x0000 && addr <= 0x1fff)
  {
    this->cpuRam[addr & CPU_RAM_MASK] = data;
  }
  else if (addr >= 0x2000 && addr <= 0x3fff)
  {
    this->ppu.cpuWrite(addr & 0x0007, data);
  }
}

void Bus::reset()
{
  this->cpu.reset();
  this->system_clock_counter = 0;
}

void Bus::clock()
{
  this->ppu.clock();
  // CPU clock is 3 times slower than ppu clock
  if(this->system_clock_counter %3 == 0){
    this->cpu.clock();
  }
  this->system_clock_counter++;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
  this->cart = cartridge;
  this->ppu.ConnectCartridge(cartridge);
}