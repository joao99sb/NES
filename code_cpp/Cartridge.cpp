#pragma once
#include "Cartridge.h"
#include <iostream>
Cartridge::Cartridge(const std::string &file_name)
{
  // iNes Format Header
  // ref: https://www.nesdev.org/wiki/INES

  struct Header
  {
    char name[4];
    uint8_t prg_rom_chunks;
    uint8_t chr_rom_chunks;
    uint8_t mapper1;
    uint8_t mapper2;
    uint8_t pgr_ram_size;
    uint8_t tv_system1;
    uint8_t tv_system2;
    char unused[5];
  } header;

  std::ifstream ifs;
  ifs.open(file_name, std::ios::binary);
  if (ifs.is_open())
  {
    // read file header
    ifs.read((char *)&header, sizeof(Header)); // read the size of Header strict and stores data of ifs in header var

    if (header.mapper1 & 0x04) ifs.seekg(512, std::ios_base::cur);
    // ifs.seekg(512, std::ios_base::cur);: This line of code uses the seekg function to move the read pointer
    // within the file. The seekg function is used to set the position of the read pointer in the input file stream (ifs).
    // Here, it moves the read pointer 512 bytes forward from the current position (std::ios_base::cur stands for "current position").
    //
    // std::ios_base::cur: is a constant defined in the standard library that represents the current position within a file or data stream.
    // This constant is mainly used with pointer manipulation functions for both input and output streams. It is commonly used with the seekg (seek get)
    // and seekp (seek put) functions of file stream classes such as std::ifstream, std::ofstream, and std::fstream.
    // When you specify std::ios_base::cur as the argument to these seek functions (seekg or seekp), it means you want to move the read or
    // write pointer from its current position in the file. In other words, the position to which the pointer will be moved is relative to the current position.

    // For example:

    // ifs.seekg(512, std::ios_base::cur); will move the read pointer 512 bytes ahead from its current position.
    // ofs.seekp(-256, std::ios_base::cur); will move the write pointer 256 bytes back from its current position.
    // This allows you to navigate the file more flexibly, reading or writing data at specific positions relative to the current position, instead of relying solely on absolute positions (beginning of the file) for these operations.

    // Determine mapper ID
    const uint8_t upperNibbleOfMapper2 = header.mapper2 >> 4;
    const uint8_t lowerNibbleOfMapper1 = header.mapper1 >> 4;
    const uint8_t shiftedUpperNibble = upperNibbleOfMapper2 << 4;
    this->mapper_id = shiftedUpperNibble | lowerNibbleOfMapper1;

    // "Discover" file format
    uint8_t file_type = 1;
    if (file_type == 0)
    {
    }
    if (file_type == 1)
    {
      this->PRG_banks = header.prg_rom_chunks;
      this->PRG_memory.resize(this->PRG_banks * _16_KB);
      ifs.read((char *)this->PRG_memory.data(), this->PRG_memory.size());

      this->CHR_banks = header.chr_rom_chunks;
      this->CHR_memory.resize(this->CHR_banks * _8_KB);
      ifs.read((char *)this->CHR_memory.data(), this->CHR_memory.size());
    }
    if (file_type == 2)
    {
    }

    // Load appropriate mapper

    switch (this->mapper_id)
    {
    case 0:
      this->mapper = std::make_shared<Mapper_0>(this->PRG_banks, this->CHR_banks);
      break;

    default:
      break;
    }
    this->image_valid = true;
    ifs.close();
  }
}


Cartridge::~Cartridge() {}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &data)
{
  uint32_t mapped_addr = 0;
  bool result =  this->mapper->cpuMapRead(addr, mapped_addr);
  if (result)
  {
    data = this->PRG_memory[mapped_addr];

    return true;
  }

  return false;
}
bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
  uint32_t mapped_addr = 0;
  if (this->mapper->cpuMapWrite(addr, mapped_addr))
  {
    this->PRG_memory[mapped_addr] = data;

    return true;
  }

  return false;
}
bool Cartridge::ppuRead(uint16_t addr, uint8_t &data)
{
  uint32_t mapped_addr = 0;
  if (this->mapper->ppuMapWrite(addr, mapped_addr))
  {
    data = this->CHR_memory[mapped_addr];
    return true;
  }
  return false;
}
bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
  uint32_t mapped_addr = 0;
  if (this->mapper->ppuMapWrite(addr, mapped_addr))
  {
    this->CHR_memory[mapped_addr] = data;
    return true;
  }
  return false;
}

bool Cartridge::ImageValid()
{
  return this->image_valid;
}