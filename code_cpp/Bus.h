#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include <array>
#include "6502/6502.h"

#define RAM_SIZE_64kB 64 * 1024

class Bus
{
public: // devices on bus
  CPU6502 cpu;

  // Fake RAM for this part
  std::array<uint8_t, RAM_SIZE_64kB> ram;

public:
  Bus(/* args */);
  ~Bus();

  void write(uint16_t addr, uint8_t value);
  uint8_t read(uint16_t addr, bool read_only = false);
};

#endif // BUS_H