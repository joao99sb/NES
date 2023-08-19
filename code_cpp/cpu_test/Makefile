TARGET = cpu_test
INCLUDE_DIR = ../ 
CPU = ../cpu

src = cpu_test.cpp Bus.cpp 

6502_SRCS = $(CPU)/6502.cpp $(CPU)/6502_ADDR_MODE.cpp $(CPU)/6502_INSTRUCTION.cpp

OBJS = $(src:.cpp=.o)
6502_OBJS = $(6502_SRCS:.cpp=.o)

AR = ar


LIB6502 = lib6502.a




LDFLAGS = -lX11 -lGL -lpng


CXX = g++
CXXFLAGS = -std=c++17 -Wall -I$(INCLUDE_DIR)

cpu: $(TARGET)

$(TARGET): $(OBJS) $(LIB6502)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIB6502) -o $@

$(LIB6502): $(6502_OBJS)
	$(AR) $(ARFLAGS) $@ $(6502_OBJS)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
