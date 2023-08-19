#pragma once
#include <cstdint>
#include <array>

#include "../cpu/6502.h"



class Bus
{
public:
	Bus();
	~Bus();

public: // Devices on bus
	CPU6502 cpu;	

	// Fake RAM for this part of the series
	std::array<uint8_t, 64 * 1024> ram;


public: // Bus Read & Write
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);
};
Bus::Bus()
{
	// Connect CPU to communication bus
	cpu.ConnectBus(this);

	// Clear RAM contents, just in case :P
	for (auto &i : ram) i = 0x00;
}


Bus::~Bus()
{
}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
	if (addr >= 0x0000 && addr <= 0xFFFF)
		ram[addr] = data;
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
	if (addr >= 0x0000 && addr <= 0xFFFF)
		return ram[addr];

	return 0x00;
}
