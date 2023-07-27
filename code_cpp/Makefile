# Nome do executável
TARGET = nes_cpu
INCLUDE_DIR = .
6502_DIR = 6502



# Lista de todos os arquivos-fonte
SRCS = nes_6502.cpp Bus.cpp
6502_SRCS = $(6502_DIR)/6502.cpp $(6502_DIR)/6502_ADDR_MODE.cpp $(6502_DIR)/6502_INSTRUCTION.cpp 


# Lista de todos os arquivos-objeto
OBJS = $(SRCS:.cpp=.o)
6502_OBJS = $(6502_SRCS:.cpp=.o)

# Comando para criar a biblioteca estática
AR = ar
ARFLAGS = rcs
LIB6502 = lib6502.a

# Flags de ligação
LDFLAGS = -lX11 -lGL -lpng

# Compilador e opções de compilação
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I$(INCLUDE_DIR)
DEBUG_CXXFLAGS = -g 


all: $(TARGET)

$(TARGET): $(OBJS) $(LIB6502)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIB6502) -o $@

$(LIB6502): $(6502_OBJS)
	$(AR) $(ARFLAGS) $@ $(6502_OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS += $(DEBUG_CXXFLAGS)
debug: $(TARGET)
	lldb ./$(TARGET)

clean:
	rm -f $(OBJS) $(6502_OBJS) $(LIB6502) $(TARGET)