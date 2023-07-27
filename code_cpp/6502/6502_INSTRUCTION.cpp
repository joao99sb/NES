#include "6502.h"



// Instruction: Add with Carry In
// Function:    A = A + M + C
// Flags Out:   C, V, N, Z
//
// Explanation:
// The purpose of this function is to add a value to the accumulator and a carry bit. If
// the result is > 255 there is an overflow setting the carry bit. Ths allows you to
// chain together ADC instructions to add numbers larger than 8-bits. This in itself is
// simple, however the 6502 supports the concepts of Negativity/Positivity and Signed Overflow.
//
// 10000100 = 128 + 4 = 132 in normal circumstances, we know this as unsigned and it allows
// us to represent numbers between 0 and 255 (given 8 bits). The 6502 can also interpret 
// this word as something else if we assume those 8 bits represent the range -128 to +127,
// i.e. it has become signed.
//
// Since 132 > 127, it effectively wraps around, through -128, to -124. This wraparound is
// called overflow, and this is a useful to know as it indicates that the calculation has
// gone outside the permissable range, and therefore no longer makes numeric sense.
//
// Note the implementation of ADD is the same in binary, this is just about how the numbers
// are represented, so the word 10000100 can be both -124 and 132 depending upon the 
// context the programming is using it in. We can prove this!
//
//  10000100 =  132  or  -124
// +00010001 = + 17      + 17
//  ========    ===       ===     See, both are valid additions, but our interpretation of
//  10010101 =  149  or  -107     the context changes the value, not the hardware!
//
// In principle under the -128 to 127 range:
// 10000000 = -128, 11111111 = -1, 00000000 = 0, 00000000 = +1, 01111111 = +127
// therefore negative numbers have the most significant set, positive numbers do not
//
// To assist us, the 6502 can set the overflow flag, if the result of the addition has
// wrapped around. V <- ~(A^M) & A^(A+M+C) :D lol, let's work out why!
//
// Let's suppose we have A = 30, M = 10 and C = 0
//          A = 30 = 00011110
//          M = 10 = 00001010+
//     RESULT = 40 = 00101000
//
// Here we have not gone out of range. The resulting significant bit has not changed.
// So let's make a truth table to understand when overflow has occurred. Here I take
// the MSB of each component, where R is RESULT.
//
// A  M  R | V | A^R | A^M |~(A^M) | 
// 0  0  0 | 0 |  0  |  0  |   1   |
// 0  0  1 | 1 |  1  |  0  |   1   |
// 0  1  0 | 0 |  0  |  1  |   0   |
// 0  1  1 | 0 |  1  |  1  |   0   |  so V = ~(A^M) & (A^R)
// 1  0  0 | 0 |  1  |  1  |   0   |
// 1  0  1 | 0 |  0  |  1  |   0   |
// 1  1  0 | 1 |  1  |  0  |   1   |
// 1  1  1 | 0 |  0  |  0  |   1   |
//
// We can see how the above equation calculates V, based on A, M and R. V was chosen
// based on the following hypothesis:
//       Positive Number + Positive Number = Negative Result -> Overflow
//       Negative Number + Negative Number = Positive Result -> Overflow
//       Positive Number + Negative Number = Either Result -> Cannot Overflow
//       Positive Number + Positive Number = Positive Result -> OK! No Overflow
//       Negative Number + Negative Number = Negative Result -> OK! NO Overflow
uint8_t CPU6502::ADC(){
  this -> fetch();
  uint16_t temp = (uint16_t)a + (uint16_t)fetched + getFlag(C);
  setFlag(C,temp > 255);
  setFlag(Z ,(temp &  0x00ff)==0);
  setFlag(N,temp &  0x08);
 	// The signed Overflow flag is set based on all that up there! :D
	setFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);
	
  this->a = temp & 0x00ff;
  return 1;

}

// Instruction: Subtraction with Borrow In
// Function:    A = A - M - (1 - C)
// Flags Out:   C, V, N, Z
//
// Explanation:
// Given the explanation for ADC above, we can reorganise our data
// to use the same computation for addition, for subtraction by multiplying
// the data by -1, i.e. make it negative
//
// A = A - M - (1 - C)  ->  A = A + -1 * (M - (1 - C))  ->  A = A + (-M + 1 + C)
//
// To make a signed positive number negative, we can invert the bits and add 1
// (OK, I lied, a little bit of 1 and 2s complement :P)
//
//  5 = 00000101
// -5 = 11111010 + 00000001 = 11111011 (or 251 in our 0 to 255 range)
//
// The range is actually unimportant, because if I take the value 15, and add 251
// to it, given we wrap around at 256, the result is 10, so it has effectively 
// subtracted 5, which was the original intention. (15 + 251) % 256 = 10
//
// Note that the equation above used (1-C), but this got converted to + 1 + C.
// This means we already have the +1, so all we need to do is invert the bits
// of M, the data(!) therfore we can simply add, exactly the same way we did 
// before.
uint8_t CPU6502::SBC(){

  fetch();
	
	// Operating in 16-bit domain to capture carry out
	
	// We can invert the bottom 8 bits with bitwise xor
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
	
	// Notice this is exactly the same as addition from here!
	uint16_t temp = (uint16_t)a + value + (uint16_t)getFlag(C);
	setFlag(C, temp & 0xFF00);
	setFlag(Z, ((temp & 0x00FF) == 0));
	setFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
	setFlag(N, temp & 0x0080);
	a = temp & 0x00FF;
	return 1;
}

// 1) Fetch the data you are working with
// 2) Perform calculation
// 3) Store the result in desired place
// 4) Set Flags of the status register
// 5) Return if instruction has potential to require additional 
//    clock cycle


// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t CPU6502::ASL()
{
	this->fetch();
	uint16_t temp = (uint16_t)fetched << 1;
	setFlag(C, (temp & 0xFF00) > 0);
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, temp & 0x80);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::BIT()
{
	fetch();
	uint16_t temp = a & fetched;
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, fetched & (1 << 7));
	setFlag(V, fetched & (1 << 6));
	return 0;
}

// Instruction: Branch if Not Equal
// Function:    if(Z == 0) pc = address
uint8_t CPU6502::BNE()
{
	if (this->getFlag(Z) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t CPU6502::BRK()
{
	pc++;
	
	setFlag(I, 1);
	write(0x0100 + stkpt, (pc >> 8) & 0x00FF);
	stkpt--;
	write(0x0100 + stkpt, pc & 0x00FF);
	stkpt--;

	setFlag(B, 1);
	write(0x0100 + stkpt, status);
	stkpt--;
	setFlag(B, 0);

	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}

// Instruction: Disable Interrupts / Clear Interrupt Flag
// Function:    I = 0
uint8_t CPU6502::CLI()
{
	setFlag(I, false);
	return 0;
}

// Instruction: Clear Overflow Flag
// Function:    V = 0
uint8_t CPU6502::CLV()
{
	setFlag(V, false);
	return 0;
}

// Instruction: Compare Accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CMP()
{
	fetch();
	uint16_t temp = (uint16_t)a - (uint16_t)fetched;
	setFlag(C, a >= fetched);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 1;
}


// Instruction: Compare X Register
// Function:    C <- X >= M      Z <- (X - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CPX()
{
	fetch();
	uint16_t temp = (uint16_t)x - (uint16_t)fetched;
	setFlag(C, x >= fetched);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CPY()
{
	fetch();
	uint16_t temp = (uint16_t)y - (uint16_t)fetched;
	setFlag(C, y >= fetched);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement Value at Memory Location
// Function:    M = M - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEC()
{
	fetch();
	uint16_t temp = fetched - 1;
	write(addr_abs, temp & 0x00FF);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEX()
{
	x--;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}

// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEY()
{
	y--;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 0;
}


// Instruction: Bitwise Logic XOR
// Function:    A = A xor M
// Flags Out:   N, Z
uint8_t CPU6502::EOR()
{
	fetch();
	a = a ^ fetched;	
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 1;
}


// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t CPU6502::INC()
{
	fetch();
	uint16_t temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	return 0;
}


// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   N, Z
uint8_t CPU6502::INX()
{
	x++;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}


// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   N, Z
uint8_t CPU6502::INY()
{
	y++;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 0;
}


// Instruction: Jump To Location
// Function:    pc = address
uint8_t CPU6502::JMP()
{
	pc = addr_abs;
	return 0;
}


// Instruction: Jump To Sub-Routine
// Function:    Push current pc to stack, pc = address
uint8_t CPU6502::JSR()
{
	pc--;

	write(0x0100 + stkpt, (pc >> 8) & 0x00FF);
	stkpt--;
	write(0x0100 + stkpt, pc & 0x00FF);
	stkpt--;

	pc = addr_abs;
	return 0;
}


// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t CPU6502::LDA()
{
	fetch();
	a = fetched;
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 1;
}


// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t CPU6502::LDX()
{
	fetch();
	x = fetched;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 1;
}


// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t CPU6502::LDY()
{
	fetch();
	y = fetched;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 1;
}

uint8_t CPU6502::LSR()
{
	fetch();
	setFlag(C, fetched & 0x0001);
	uint16_t temp = fetched >> 1;	
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::NOP()
{
	// Sadly not all NOPs are equal, Ive added a few here
	// based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
	// and will add more based on game compatibility, and ultimately
	// I'd like to cover all illegal opcodes too
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}
// Instruction: Bitwise Logic AND
// Function:    A = A & M
// Flags Out:   N, Z
uint8_t CPU6502::AND(){
  this->fetch();
  this->a &= this->fetched;
  this->setFlag(this->Z,this->a==0x00);
  this->setFlag(this->N,this->a==0x80);

  return 1; 

}

// Instruction: Branch if Carry Set
// Function:    if(C == 1) pc = address
uint8_t CPU6502::BCS(){

  if(this->getFlag(C) == 1){
    this->cycles++;
    this->addr_abs = this->pc + this->addr_rel;

    if((this->addr_abs & 0xff00) != (this->pc & 0xff00) ){
      this->cycles++;
    }

    this->pc = this->addr_abs;
  }

  return 0;

}

// Instruction: Branch if Carry Set
// Function:    if(C == 1) pc = address
uint8_t CPU6502::BCC(){

  if(this->getFlag(C) == 0){
    this->cycles++;
    this->addr_abs = this->pc + this->addr_rel;

    if((this->addr_abs & 0xff00) != (this->pc & 0xff00) ){
      this->cycles++;
    }

    this->pc = this->addr_abs;
  }

  return 0;

}

// Instruction: Branch if Equal
// Function:    if(Z == 1) pc = address
uint8_t CPU6502::BEQ(){

	if (this->getFlag(Z) == 1)
	{
		this->cycles++;
		this->addr_abs = pc + addr_rel;

		if ((this->addr_abs & 0xFF00) != (this->pc & 0xFF00))
			cycles++;

		this->pc = this->addr_abs;
	}
	return 0;

}

// Instruction: Branch if Negative
// Function:    if(N == 1) pc = address
uint8_t CPU6502::BMI()
{
	if (this->getFlag(N) == 1)
	{
	this->cycles++;
		this->addr_abs = pc + addr_rel;

		if ((this->addr_abs & 0xFF00) != (this->pc & 0xFF00))
			cycles++;

		this->pc = this->addr_abs;
	}
	return 0;
}

// Instruction: Branch if Positive
// Function:    if(N == 0) pc = address
uint8_t CPU6502::BPL(){
  	if (this->getFlag(N) == 0)
	{
	this->cycles++;
		this->addr_abs = pc + addr_rel;

		if ((this->addr_abs & 0xFF00) != (this->pc & 0xFF00))
			cycles++;

		this->pc = this->addr_abs;
	}
	return 0;
}

// Instruction: Branch if Overflow Clear
// Function:    if(V == 0) pc = address
uint8_t CPU6502::BVC(){
  	if (this->getFlag(V) == 0)
	{
	this->cycles++;
		this->addr_abs = pc + addr_rel;

		if ((this->addr_abs & 0xFF00) != (this->pc & 0xFF00))
			cycles++;

		this->pc = this->addr_abs;
	}
	return 0;
}

// Instruction: Branch if Overflow Set
// Function:    if(V == 1) pc = address
uint8_t CPU6502::BVS(){
  	if (this->getFlag(V) == 1)
	{
	this->cycles++;
		this->addr_abs = pc + addr_rel;

		if ((this->addr_abs & 0xFF00) != (this->pc & 0xFF00))
			cycles++;

		this->pc = this->addr_abs;
	}
	return 0;
}

// Instruction: Clear Carry Flag
// Function:    C = 0
uint8_t CPU6502::CLC(){

  this->setFlag(C,false);
  return 0;

}

// Instruction: Clear Decimal Flag
// Function:    D = 0
uint8_t CPU6502::CLD(){

  this->setFlag(D,false);
  return 0;

}



// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t CPU6502::ORA()
{
	fetch();
	a = a | fetched;
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 1;
}


// Instruction: Push Accumulator to Stack
// Function:    A -> stack
uint8_t CPU6502::PHA()
{
	write(0x0100 + stkpt, a);
	stkpt--;
	return 0;
}


// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t CPU6502::PHP()
{
	write(0x0100 + stkpt, status | B | U);
	setFlag(B, 0);
	setFlag(U, 0);
	stkpt--;
	return 0;
}


// Instruction: Pop Accumulator off Stack
// Function:    A <- stack
// Flags Out:   N, Z
uint8_t CPU6502::PLA()
{
	stkpt++;
	a = read(0x0100 + stkpt);
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 0;
}


// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t CPU6502::PLP()
{
	stkpt++;
	status = read(0x0100 + stkpt);
	setFlag(U, 1);
	return 0;
}

uint8_t CPU6502::ROL()
{
	fetch();
	uint16_t temp = (uint16_t)(fetched << 1) | getFlag(C);
	setFlag(C, temp & 0xFF00);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::ROR()
{
	fetch();
	uint16_t temp = (uint16_t)(getFlag(C) << 7) | (fetched >> 1);
	setFlag(C, fetched & 0x01);
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &CPU6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::RTI()
{
	stkpt++;
	status = read(0x0100 + stkpt);
	status &= ~B;
	status &= ~U;

	stkpt++;
	pc = (uint16_t)read(0x0100 + stkpt);
	stkpt++;
	pc |= (uint16_t)read(0x0100 + stkpt) << 8;
	return 0;
}

uint8_t CPU6502::RTS()
{
	stkpt++;
	pc = (uint16_t)read(0x0100 + stkpt);
	stkpt++;
	pc |= (uint16_t)read(0x0100 + stkpt) << 8;
	
	pc++;
	return 0;
}




// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t CPU6502::SEC()
{
	setFlag(C, true);
	return 0;
}


// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t CPU6502::SED()
{
	setFlag(D, true);
	return 0;
}


// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t CPU6502::SEI()
{
	setFlag(I, true);
	return 0;
}


// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t CPU6502::STA()
{
	write(addr_abs, a);
	return 0;
}


// Instruction: Store X Register at Address
// Function:    M = X
uint8_t CPU6502::STX()
{
	write(addr_abs, x);
	return 0;
}


// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t CPU6502::STY()
{
	write(addr_abs, y);
	return 0;
}


// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t CPU6502::TAX()
{
	x = a;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}


// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t CPU6502::TAY()
{
	y = a;
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);
	return 0;
}


// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t CPU6502::TSX()
{
	x = stkpt;
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t CPU6502::TXA()
{
	a = x;
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 0;
}


// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t CPU6502::TXS()
{
	stkpt = x;
	return 0;
}


// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t CPU6502::TYA()
{
	a = y;
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);
	return 0;
}


// This function captures illegal opcodes
uint8_t CPU6502::XXX()
{
	return 0;
}