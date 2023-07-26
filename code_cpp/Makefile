default:
	g++ 6502.cpp  Bus.cpp  nes_6502.cpp  -o nes_cpu -lX11 -lGL -lpng

debug: 6502.cpp Bus.cpp nes_6502.cpp
	g++ -g 6502.cpp Bus.cpp nes_6502.cpp -o debug_file -lX11 -lGL -lpng
	lldb ./debug_file
	rm ./debug_file