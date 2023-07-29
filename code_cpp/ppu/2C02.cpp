#include "2C02.h"

PPU2C02::PPU2C02()
{
  pal_screen[0x00] = olc::Pixel(84, 84, 84);
  pal_screen[0x01] = olc::Pixel(0, 30, 116);
  pal_screen[0x02] = olc::Pixel(8, 16, 144);
  pal_screen[0x03] = olc::Pixel(48, 0, 136);
  pal_screen[0x04] = olc::Pixel(68, 0, 100);
  pal_screen[0x05] = olc::Pixel(92, 0, 48);
  pal_screen[0x06] = olc::Pixel(84, 4, 0);
  pal_screen[0x07] = olc::Pixel(60, 24, 0);
  pal_screen[0x08] = olc::Pixel(32, 42, 0);
  pal_screen[0x09] = olc::Pixel(8, 58, 0);
  pal_screen[0x0A] = olc::Pixel(0, 64, 0);
  pal_screen[0x0B] = olc::Pixel(0, 60, 0);
  pal_screen[0x0C] = olc::Pixel(0, 50, 60);
  pal_screen[0x0D] = olc::Pixel(0, 0, 0);
  pal_screen[0x0E] = olc::Pixel(0, 0, 0);
  pal_screen[0x0F] = olc::Pixel(0, 0, 0);

  pal_screen[0x10] = olc::Pixel(152, 150, 152);
  pal_screen[0x11] = olc::Pixel(8, 76, 196);
  pal_screen[0x12] = olc::Pixel(48, 50, 236);
  pal_screen[0x13] = olc::Pixel(92, 30, 228);
  pal_screen[0x14] = olc::Pixel(136, 20, 176);
  pal_screen[0x15] = olc::Pixel(160, 20, 100);
  pal_screen[0x16] = olc::Pixel(152, 34, 32);
  pal_screen[0x17] = olc::Pixel(120, 60, 0);
  pal_screen[0x18] = olc::Pixel(84, 90, 0);
  pal_screen[0x19] = olc::Pixel(40, 114, 0);
  pal_screen[0x1A] = olc::Pixel(8, 124, 0);
  pal_screen[0x1B] = olc::Pixel(0, 118, 40);
  pal_screen[0x1C] = olc::Pixel(0, 102, 120);
  pal_screen[0x1D] = olc::Pixel(0, 0, 0);
  pal_screen[0x1E] = olc::Pixel(0, 0, 0);
  pal_screen[0x1F] = olc::Pixel(0, 0, 0);

  pal_screen[0x20] = olc::Pixel(236, 238, 236);
  pal_screen[0x21] = olc::Pixel(76, 154, 236);
  pal_screen[0x22] = olc::Pixel(120, 124, 236);
  pal_screen[0x23] = olc::Pixel(176, 98, 236);
  pal_screen[0x24] = olc::Pixel(228, 84, 236);
  pal_screen[0x25] = olc::Pixel(236, 88, 180);
  pal_screen[0x26] = olc::Pixel(236, 106, 100);
  pal_screen[0x27] = olc::Pixel(212, 136, 32);
  pal_screen[0x28] = olc::Pixel(160, 170, 0);
  pal_screen[0x29] = olc::Pixel(116, 196, 0);
  pal_screen[0x2A] = olc::Pixel(76, 208, 32);
  pal_screen[0x2B] = olc::Pixel(56, 204, 108);
  pal_screen[0x2C] = olc::Pixel(56, 180, 204);
  pal_screen[0x2D] = olc::Pixel(60, 60, 60);
  pal_screen[0x2E] = olc::Pixel(0, 0, 0);
  pal_screen[0x2F] = olc::Pixel(0, 0, 0);

  pal_screen[0x30] = olc::Pixel(236, 238, 236);
  pal_screen[0x31] = olc::Pixel(168, 204, 236);
  pal_screen[0x32] = olc::Pixel(188, 188, 236);
  pal_screen[0x33] = olc::Pixel(212, 178, 236);
  pal_screen[0x34] = olc::Pixel(236, 174, 236);
  pal_screen[0x35] = olc::Pixel(236, 174, 212);
  pal_screen[0x36] = olc::Pixel(236, 180, 176);
  pal_screen[0x37] = olc::Pixel(228, 196, 144);
  pal_screen[0x38] = olc::Pixel(204, 210, 120);
  pal_screen[0x39] = olc::Pixel(180, 222, 120);
  pal_screen[0x3A] = olc::Pixel(168, 226, 144);
  pal_screen[0x3B] = olc::Pixel(152, 226, 180);
  pal_screen[0x3C] = olc::Pixel(160, 214, 228);
  pal_screen[0x3D] = olc::Pixel(160, 162, 160);
  pal_screen[0x3E] = olc::Pixel(0, 0, 0);
  pal_screen[0x3F] = olc::Pixel(0, 0, 0);

  spr_screen = std::make_unique<olc::Sprite>(256, 240);
  spr_name_table[0] = std::make_unique<olc::Sprite>(256, 240);
  spr_name_table[1] = std::make_unique<olc::Sprite>(256, 240);
  spr_pattern_table[0] = std::make_unique<olc::Sprite>(128, 128);
  spr_pattern_table[1] = std::make_unique<olc::Sprite>(128, 128);
}

PPU2C02::~PPU2C02() {}

uint8_t PPU2C02::cpuRead(uint16_t addr, bool read_only)
{
  uint8_t data = 0x00;

  switch (addr)
  {
  case 0x0000: // Control

    break;
  case 0x0001: // Mask
    break;
  case 0x0002: // Status
    break;
  case 0x0003: // OAM Address
    break;
  case 0x0004: // OAM Data
    break;
  case 0x0005: // Scroll
    break;
  case 0x0006: // PPU Address
    break;
  case 0x0007: // PPU Data
    break;
  }

  return data;
}
uint8_t PPU2C02::cpuWrite(uint16_t addr, uint8_t value)
{

  switch (addr)
  {
  case 0x0000: // Control
    break;
  case 0x0001: // Mask
    break;
  case 0x0002: // Status
    break;
  case 0x0003: // OAM Address
    break;
  case 0x0004: // OAM Data
    break;
  case 0x0005: // Scroll

  case 0x0006: // PPU Address

    break;
  case 0x0007: // PPU Data

    break;
  }
}

uint8_t PPU2C02::ppuRead(uint16_t addr, bool read_only)
{
  uint8_t data = 0x00;

  addr &= 0x3FFF;

  if (this->cart->ppuRead(addr, data))
  {
  }
  return data;
}
void PPU2C02::ppuWrite(uint16_t addr, uint8_t data)
{
  addr &= 0x3FFF;
  if (this->cart->ppuWrite(addr, data))
  {
  }
}

void PPU2C02::clock()
{
  // FAKE some noise for now
  int pixelColor = (rand() % 2) ? 0x3F : 0x30;
  spr_screen->SetPixel(cycle - 1, scanline, pal_screen[pixelColor]);
  // advance rendere - it never stops, it's relentless
  this->cycle++;
  if (this->cycle >= 341)
  {
    this->cycle = 0;
    this->scanline++;
    if (this->scanline >= 261)
    {
      this->scanline = -1;
      this->frame_complete = true;
    }
  }
}

void PPU2C02::ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
  this->cart = cartridge;
}

olc::Sprite &PPU2C02::getScreen()
{
  return *this->spr_screen;
}

olc::Sprite &PPU2C02::getNameTable(uint8_t index)
{
  return *this->spr_name_table[index];
}

olc::Sprite &PPU2C02::getPatternTable(uint8_t index)
{
  return *this->spr_pattern_table[index];
}
