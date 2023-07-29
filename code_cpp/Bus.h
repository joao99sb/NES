#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include <array>
#include <memory>
#include "cpu/6502.h"
#include "ppu/2C02.h"
#include "Cartridge.h"
#include "Constants.h"

#define CPU_RAM_SIZE_2kB _2_KB

// serve para que o valor aplicado do endereço não utrapace 2kb [0x0000 - 0x07FF]
#define CPU_RAM_MASK 0x07FF

class Bus
{
public:
  Bus();
  ~Bus();

public: // devices on bus
  CPU6502 cpu;
  PPU2C02 ppu;

  // Fake RAM for this part
  std::array<uint8_t, CPU_RAM_SIZE_2kB > cpuRam;

  // The Cartridge
  std::shared_ptr<Cartridge> cart;

public:
  // Bus Read & Write
  void cpuWrite(uint16_t addr, uint8_t value);
  uint8_t cpuRead(uint16_t addr, bool read_only = false);

  // System interface
  void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);
  void reset();
  void clock();

private:
  // A count of how many clocks have passed
  uint32_t system_clock_counter = 0;
};

#endif // BUS_H