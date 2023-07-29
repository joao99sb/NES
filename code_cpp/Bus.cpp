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
  uint16_t mirroredAddr = addr & CPU_RAM_MASK;   // The address with the bottom 11 bits masked for mirrored RAM
  uint8_t ppuRegisterAddr = addr & PPU_RAM_MASK; // The address with the bottom 3 bits masked for PPU register

  // Check if the cartridge can handle the read operation for the given address.
  if (this->cart->cpuRead(addr, data))
  {
    // Cartridge Address Range
    // The cartridge handles the read operation, so the data
    // variable should be updated accordingly by the cartridge.
    // The cartridge may handle memory-mapped registers or
    // other custom functionality.
  }
  // If the cartridge does not handle the read operation, we
  // check other address ranges.
  else if (addr >= 0x0000 && addr <= 0x1fff)
  {
    // System RAM Address Range.
    // The range covers 8KB, but there is only 2KB available.
    // That 2KB is "mirrored" through this address range.
    // Using bitwise AND to mask the bottom 11 bits is the
    // same as addr % 2048.
    data = this->cpuRam[mirroredAddr];
  }
  else if (addr >= 0x2000 && addr <= 0x3fff)
  {
    // PPU (Picture Processing Unit) Address Range.
    // The PPU has some memory-mapped registers that can be
    // read at specific addresses. However, the PPU has only
    // 8 registers, so we use a bitwise AND to mask the bottom
    // 3 bits (0x0007) to keep the address within the valid
    // range of 0 to 7.
    data = this->ppu.cpuRead(ppuRegisterAddr, read_only);
  }

  return data;
}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
  uint16_t mirroredAddr = addr & CPU_RAM_MASK;   // The address with the bottom 11 bits masked for mirrored RAM
  uint8_t ppuRegisterAddr = addr & PPU_RAM_MASK; // The address with the bottom 3 bits masked for PPU register

  // Check if the cartridge can handle the write operation for the given address.
  if (this->cart->cpuWrite(addr, data))
  {
    // Cartridge Address Range
    // The cartridge handles the write operation, so the data
    // will be handled accordingly by the cartridge.
    // The cartridge may handle memory-mapped registers or
    // other custom functionality.
  }
  // If the cartridge does not handle the write operation, we
  // check other address ranges.
  else if (addr >= 0x0000 && addr <= 0x1fff)
  {
    // System RAM Address Range.
    // The range covers 8KB, but there is only 2KB available.
    // That 2KB is "mirrored" through this address range.
    // Using bitwise AND to mask the bottom 11 bits is the
    // same as addr % 2048.
    this->cpuRam[mirroredAddr] = data;
  }
  else if (addr >= 0x2000 && addr <= 0x3fff)
  {
    // PPU (Picture Processing Unit) Address Range.
    // The PPU has some memory-mapped registers that can be
    // written at specific addresses. However, the PPU has only
    // 8 registers, so we use a bitwise AND to mask the bottom
    // 3 bits (0x0007) to keep the address within the valid
    // range of 0 to 7.
    this->ppu.cpuWrite(ppuRegisterAddr, data);
  }
}

void Bus::reset()
{
  this->cpu.reset();
  this->system_clock_counter = 0;
}

void Bus::clock()
{
  // Clock the PPU (Picture Processing Unit)
  this->ppu.clock();

  // The CPU clock is 3 times slower than the PPU clock.
  // We use the system_clock_counter to keep track of the
  // number of cycles and determine when to clock the CPU.
  // When system_clock_counter is a multiple of 3, we clock the CPU.
  if (this->system_clock_counter % 3 == 0)
  {
    this->cpu.clock();
  }

  // Increment the system_clock_counter for the next cycle.
  this->system_clock_counter++;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
  this->cart = cartridge;
  this->ppu.ConnectCartridge(cartridge);
}