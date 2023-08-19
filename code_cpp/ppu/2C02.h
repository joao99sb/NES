#pragma once
#include <cstdint>
#include <memory>
#include "../Cartridge.h"
#include "../game_engine.h"

class PPU2C02
{

public:
  PPU2C02(/* args */);
  ~PPU2C02();

  // Communication withc Main Bus
  uint8_t cpuRead(uint16_t addr, bool read_only = false);
  uint8_t cpuWrite(uint16_t addr, uint8_t value);

  // Communication with PPU Bus
  uint8_t ppuRead(uint16_t addr, bool read_only = false);
  void ppuWrite(uint16_t addr, uint8_t value);

  // Interface
  void ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge);
  void clock();

private:
  // The cartridge
  std::shared_ptr<Cartridge> cart;

  uint8_t table_name[2][KB];
  uint8_t table_pattern[2][_4_KB];
  uint8_t table_palette[32];

private:
  // palette ref: https://www.nesdev.org/wiki/PPU_palettes
  olc::Pixel pal_screen[0x40];

  // change to smart pointer
  std::unique_ptr<olc::Sprite> spr_screen;
  std::unique_ptr<olc::Sprite> spr_name_table[2];
  std::unique_ptr<olc::Sprite> spr_pattern_table[2];

public:
  // debug Utils
  olc::Sprite &getScreen();
  olc::Sprite &getNameTable(uint8_t index);
  olc::Sprite &getPatternTable(uint8_t index,uint8_t palette);
	olc::Pixel  &getColourFromPaletteRam(uint8_t palette, uint8_t pixel);
  bool frame_complete = false;

private:
  int16_t scanline = 0;
  int16_t cycle = 0;
};
