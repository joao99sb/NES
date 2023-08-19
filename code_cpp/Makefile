# Nome do executável
TARGET = nes_cpu
INCLUDE_DIR = .
CPU = cpu
PPU = ppu
MAPPERS = mappers

# Lista de todos os arquivos-fonte
SRCS = nes_6502.cpp Bus.cpp Cartridge.cpp
6502_SRCS = $(CPU)/6502.cpp $(CPU)/6502_ADDR_MODE.cpp $(CPU)/6502_INSTRUCTION.cpp 
PPU_SRSC = $(PPU)/2C02.cpp
MAPPERS_SRSC = $(MAPPERS)/Mapper_0.cpp $(MAPPERS)/Mapper.cpp

# Lista de todos os arquivos-objeto
OBJS = $(SRCS:.cpp=.o)
6502_OBJS = $(6502_SRCS:.cpp=.o)
PPU_OBJS = $(PPU_SRSC:.cpp=.o)
MAPPERS_OBJS = $(MAPPERS_SRSC:.cpp=.o)

# Comando para criar a biblioteca estática
AR = ar
ARFLAGS = rcs
LIB6502 = lib6502.a
LIBPPU = libPPU.a
LIBMAPPER = libMAPPER.a
# Flags de ligação
LDFLAGS = -lX11 -lGL -lpng

# Compilador e opções de compilação
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I$(INCLUDE_DIR)
DEBUG_CXXFLAGS = -g 


cpu: $(TARGET)

$(TARGET): $(OBJS) $(LIB6502) $(LIBMAPPER) $(LIBPPU)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIB6502) $(LIBMAPPER) $(LIBPPU) -o $@

$(LIB6502): $(6502_OBJS)
	$(AR) $(ARFLAGS) $@ $(6502_OBJS)

$(LIBPPU): $(PPU_OBJS)
	$(AR) $(ARFLAGS) $@ $(PPU_OBJS)
	
$(LIBMAPPER): $(MAPPERS_OBJS)
	$(AR) $(ARFLAGS) $@ $(MAPPERS_OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Debug
debug: CXXFLAGS += $(DEBUG_CXXFLAGS)
debug: $(TARGET)


#Cpu teste
CPU_TEST = cpu_t

CT_SRC = cpu_test/cpu_test.cpp 
CT_OBJS = $(CT_SRC:.cpp=.o)

CT_INCLUDE= ./cpu_test

CT_CXXFLAGS = -std=c++17 -Wall -I$(CT_INCLUDE) -I$(INCLUDE_DIR)
cpu_test: $(CPU_TEST)

$(CPU_TEST):$(CT_OBJS) $(LIB6502) 
	$(CXX) $(LDFLAGS) $(CT_OBJS) $(LIB6502) -o $@

%.o: %.cpp
	$(CXX) $(CT_CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(6502_OBJS)  $(PPU_OBJS) $(MAPPERS_OBJS) $(LIB6502) $(LIBMAPPER) $(LIBPPU) $(TARGET) $(CT_OBJS) $(CPU_TEST)




