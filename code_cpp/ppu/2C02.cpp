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
  else if (addr >= 0x0000 && addr <= 0x1FFF)
  {
    uint16_t table_pattern_offset = addr & 0x0FFF;
    uint16_t table_pattern_high_bits_shifted = addr >> 12;
    uint8_t table_pattern_high_bits = table_pattern_high_bits_shifted & 0x0F;
    data = this->table_pattern[table_pattern_high_bits][table_pattern_offset];
  }
  else if (addr >= 0x2000 && addr <= 0x3EFF)
  {
  }

  else if (addr >= 0x3F00 && addr <= 0x3EFF)
  {
    addr &= 0x001F;
    if (addr == 0x0010)
      addr = 0x0000;
    if (addr == 0x0014)
      addr = 0x0004;
    if (addr == 0x0018)
      addr = 0x0008;
    if (addr == 0x001C)
      addr = 0x000C;

    data = this->table_palette[addr];
  }
  return data;
}
void PPU2C02::ppuWrite(uint16_t addr, uint8_t data)
{
  addr &= 0x3FFF;
  if (this->cart->ppuWrite(addr, data))
  {
  }
  else if (addr >= 0x0000 && addr <= 0x1FFF)
  {
    uint16_t table_pattern_offset = addr & 0x0FFF;
    uint16_t table_pattern_high_bits_shifted = addr >> 12;
    uint8_t table_pattern_high_bits = table_pattern_high_bits_shifted & 0x0F;
    this->table_pattern[table_pattern_high_bits][table_pattern_offset] = data;
  }
  else if (addr >= 0x2000 && addr <= 0x3EFF)
  {
  }

  else if (addr >= 0x3F00 && addr <= 0x3EFF)
  {
    addr &= 0x001F;
    if (addr == 0x0010)
      addr = 0x0000;
    if (addr == 0x0014)
      addr = 0x0004;
    if (addr == 0x0018)
      addr = 0x0008;
    if (addr == 0x001C)
      addr = 0x000C;

    this->table_palette[addr] = data;
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

olc::Sprite &PPU2C02::getPatternTable(uint8_t index, uint8_t palette)
{
  // This function draw the CHR ROM for a given pattern table into
  // an olc::Sprite, using a specified palette. Pattern tables consist
  // of 16x16 "tiles or characters". It is independent of the running
  // emulation and using it does not change the systems state, though
  // it gets all the data it needs from the live system. Consequently,
  // if the game has not yet established palettes or mapped to relevant
  // CHR ROM banks, the sprite may look empty. This approach permits a
  // "live" extraction of the pattern table exactly how the NES, and
  // ultimately the player would see it.

  // A tile consists of 8x8 pixels. On the NES, pixels are 2 bits, which
  // gives an index into 4 different colours of a specific palette. There
  // are 8 palettes to choose from. Colour "0" in each palette is effectively
  // considered transparent, as those locations in memory "mirror" the global
  // background colour being used. This mechanics of this are shown in
  // detail in ppuRead() & ppuWrite()

  // Characters on NES
  // ~~~~~~~~~~~~~~~~~
  // The NES stores characters using 2-bit pixels. These are not stored sequentially
  // but in singular bit planes. For example:
  //
  // 2-Bit Pixels       LSB Bit Plane     MSB Bit Plane
  // 0 0 0 0 0 0 0 0	  0 0 0 0 0 0 0 0   0 0 0 0 0 0 0 0
  // 0 1 1 0 0 1 1 0	  0 1 1 0 0 1 1 0   0 0 0 0 0 0 0 0
  // 0 1 2 0 0 2 1 0	  0 1 1 0 0 1 1 0   0 0 1 0 0 1 0 0
  // 0 0 0 0 0 0 0 0 =  0 0 0 0 0 0 0 0 + 0 0 0 0 0 0 0 0
  // 0 1 1 0 0 1 1 0	  0 1 1 0 0 1 1 0   0 0 0 0 0 0 0 0
  // 0 0 1 1 1 1 0 0	  0 0 1 1 1 1 0 0   0 0 0 0 0 0 0 0
  // 0 0 0 2 2 0 0 0	  0 0 0 1 1 0 0 0   0 0 0 1 1 0 0 0
  // 0 0 0 0 0 0 0 0	  0 0 0 0 0 0 0 0   0 0 0 0 0 0 0 0
  //
  // The planes are stored as 8 bytes of LSB, followed by 8 bytes of MSB

  // Loop through all 16x16 tiles
  for (uint16_t tile_y = 0; tile_y < 16; tile_y++)
  {
    for (uint16_t tile_x = 0; tile_x < 16; tile_x++)
    {
      // Convert the 2D tile coordinate into a 1D offset into the pattern
      // table memory.
      uint16_t off_set = tile_y * 256 + tile_x * 16;

      // Now loop through 8 rows of 8 pixels
      for (uint16_t row = 0; row < 8; row++)
      {
        // For each row, we need to read both bit planes of the character
        // in order to extract the least significant and most significant
        // bits of the 2 bit pixel value. in the CHR ROM, each character
        // is stored as 64 bits of lsb, followed by 64 bits of msb. This
        // conveniently means that two corresponding rows are always 8
        // bytes apart in memory.
        uint8_t tile_lsb = this->ppuRead(index * 0x1000 + off_set + row + 0);
        uint8_t tile_msb = this->ppuRead(index * 0x1000 + off_set + row + 8);

        // Now we have a single row of the two bit planes for the character
        // we need to iterate through the 8-bit words, combining them to give
        // us the final pixel index
        for (uint16_t col = 0; col < 8; col++)
        {
          // We can get the index value by simply adding the bits together
          // but we're only interested in the lsb of the row words because...
          uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x1);

          // ...we will shift the row words 1 bit right for each column of
          // the character.
          tile_lsb >>= 1;
          tile_msb >>= 1;

          (*this->spr_pattern_table[index]).SetPixel(tile_x * 8 + (7 - col), // Because we are using the lsb of the row word first
                                                                             // we are effectively reading the row from right
                                                                             // to left, so we need to draw the row "backwards"
                                                     tile_y * 8 + row, this->getColourFromPaletteRam(palette, pixel));
        }
      }
    }
  }

  return *this->spr_pattern_table[index];
}

olc::Pixel &PPU2C02::getColourFromPaletteRam(uint8_t palette, uint8_t pixel)
{
  // This is a convenience function that takes a specified palette and pixel
  // index and returns the appropriate screen colour.
  // "0x3F00"       - Offset into PPU addressable range where palettes are stored
  // "palette << 2" - Each palette is 4 bytes in size
  // "pixel"        - Each pixel index is either 0, 1, 2 or 3
  // "& 0x3F"       - Stops us reading beyond the bounds of the palScreen array
  return pal_screen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];

  // Note: We dont access tblPalette directly here, instead we know that ppuRead()
  // will map the address onto the seperate small RAM attached to the PPU bus.
}