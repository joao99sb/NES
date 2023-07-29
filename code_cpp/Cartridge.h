#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include "Constants.h"
#include "mappers/Mapper_0.h"

class Cartridge
{

public:
  Cartridge(const std::string &file_name);
  ~Cartridge();

  // Communication withc Main Bus
  bool cpuRead(uint16_t addr, uint8_t &value);
  bool cpuWrite(uint16_t addr, uint8_t value);

  // Communication with PPU Bus
  bool ppuRead(uint16_t addr, uint8_t &value);
  bool ppuWrite(uint16_t addr, uint8_t value);

  bool ImageValid();

private:
  bool image_valid = false;

  std::vector<uint8_t> PRG_memory;
  std::vector<uint8_t> CHR_memory;

  uint8_t mapper_id = 0;
  uint8_t PRG_banks = 0;
  uint8_t CHR_banks = 0;

  std::shared_ptr<Mapper> mapper;
};

//
// On a NES (Nintendo Entertainment System) cartridge, "PRG" and "CHR" stand for the following:

// 1) PRG: Program
//   PRG stands for "Program." It refers to the part of the NES cartridge that contains the actual program code of the game.
//   his code is responsible for running the gameplay, logic, and all the functions of the game.
//   When you insert a NES cartridge into the console and power it on, the NES reads the PRG portion of the cartridge and executes the program to run the game.

// 2) CHR: Character
//  CHR stands for "Character." It refers to the part of the NES cartridge that contains graphical data, such as sprites, tiles,
//  and backgrounds used by the game to display graphics on the screen.
//  The NES reads this data from the CHR portion of the cartridge to create the visual elements you see while playing the game.

// Both PRG and CHR are crucial components of an NES cartridge, working together to provide the gaming experience on the NES console.
// The PRG contains the code that controls the gameplay, while the CHR contains the graphics and visual assets necessary for the game's appearance.
//