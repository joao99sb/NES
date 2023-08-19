#include <cstdint>
#include <sstream>
#include <string>

#include "./Bus.h"
#include "../cpu/6502.h"

#define OLC_PGE_APPLICATION
#include "../game_engine.h"


class CPU6502_Demo : public olc::PixelGameEngine {
 public:
  CPU6502_Demo() { sAppName = "CPU Test"; }

  Bus nes;
  std::map<uint16_t, std::string> mapAsm;

  std::string hex(uint32_t n, uint8_t d) {
    std::string s(d, '0');

    for (int i = d - 1; i >= 0; i--, n >>= 4) {
      s[i] = "0123456789ABCDEF"[n & 0xF];
    }
    return s;
  }

  void DrawRam(int x, int y, uint16_t addr, int Rows, int columns) {
    int ramX = x, ramY = y;

    for (int row = 0; row < Rows; row++) {
      std::string offset = "$" + hex(addr, 4) + ":";

      for (int col = 0; col < columns; col++) {
        offset += " " + hex(nes.cpuRead(addr, true), 2);
        addr += 1;
      }

      DrawString(ramX, ramY, offset);
      ramY += 10;
    }
  }

  void DrawCpu(int x, int y) {
    std::string status = "STATUS: ";
    DrawString(x, y, "STATUS:", olc::WHITE);
    DrawString(x + 64, y, "N",
               nes.cpu.status & CPU6502::N ? olc::GREEN : olc::RED);

    DrawString(x + 80, y, "V",
               nes.cpu.status & CPU6502::V ? olc::GREEN : olc::RED);

    DrawString(x + 96, y, "-",
               nes.cpu.status & CPU6502::U ? olc::GREEN : olc::RED);
    DrawString(x + 112, y, "B",
               nes.cpu.status & CPU6502::B ? olc::GREEN : olc::RED);
    DrawString(x + 128, y, "D",
               nes.cpu.status & CPU6502::D ? olc::GREEN : olc::RED);
    DrawString(x + 144, y, "I",
               nes.cpu.status & CPU6502::I ? olc::GREEN : olc::RED);
    DrawString(x + 160, y, "Z",
               nes.cpu.status & CPU6502::Z ? olc::GREEN : olc::RED);
    DrawString(x + 178, y, "C",
               nes.cpu.status & CPU6502::C ? olc::GREEN : olc::RED);
    DrawString(x, y + 10, "PC: $" + hex(nes.cpu.pc, 4));
    DrawString(
        x, y + 20,
        "A: $" + hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
    DrawString(
        x, y + 30,
        "X: $" + hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
    DrawString(
        x, y + 40,
        "Y: $" + hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
    DrawString(x, y + 50, "Stack P: $" + hex(nes.cpu.stkpt, 4));
  }

  void DrawCode(int x, int y, int lines) {
    auto it_a = mapAsm.find(nes.cpu.pc);
    int lineY = (lines >> 1) * 10 + y;

    if (it_a != mapAsm.end()) {
      DrawString(x, lineY, it_a->second, olc::CYAN);

      while (lineY < (lines * 10) + y) {
        lineY += 10;
        if (++it_a != mapAsm.end()) {
          DrawString(x, lineY, it_a->second);
        }
      }
    }
    it_a = mapAsm.find(nes.cpu.pc);
    lineY = (lines >> 1) * 10 + y;
    if (it_a != mapAsm.end()) {
      while (lineY > y) {
        lineY -= 10;
        if (--it_a != mapAsm.end()) {
          DrawString(x, lineY, it_a->second);
        }
      }
    }
  }

  bool OnUserCreate() {
    // Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
    /*
            *=$8000
            LDX #10
            STX $0000
            LDX #3
            STX $0001
            LDY $0000
            LDA #0
            CLC
            loop
            ADC $0001
            DEY
            BNE loOp
            STA $0002
            NOP
            NOP
            NOP
    */

    std::stringstream ss;
    ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA "
          "8D 02 00 EA EA EA";
    uint16_t offset = 0x8000;

    while (!ss.eof()) {
      std::string b;
      ss >> b;
      nes.ram[offset++] = (uint8_t)std::stoul(b, nullptr, 16);
    }
    nes.ram[0xFFFC] = 0x00;
    nes.ram[0xFFFD] = 0x80;

    mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

    nes.cpu.reset();
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) {
    Clear(olc::DARK_BLUE);

    if (GetKey(olc::Key::SPACE).bPressed) {
      do {
        nes.cpu.clock();
      } while (!nes.cpu.complete());
    }

    if (GetKey(olc::Key::R).bPressed) nes.cpu.reset();

    if (GetKey(olc::Key::I).bPressed) nes.cpu.irq();

    if (GetKey(olc::Key::N).bPressed) nes.cpu.nmi();

    // Draw Ram Page 0x00
    DrawRam(2, 2, 0x0000, 16, 16);
    DrawRam(2, 182, 0x8000, 16, 16);
    DrawCpu(448, 2);
    DrawCode(448, 72, 26);

    DrawString(10, 370,
               "SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI");

    return true;
  }
};

int main() {
  CPU6502_Demo cpu;
  cpu.Construct(680, 480, 2, 2);
  cpu.Start();

  return 0;
}
