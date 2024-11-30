#include"chip8.h"

CHIP8::CHIP8() {
	programCounter = START_ADDRESS;

	for (int i = 0; i < FONTSET_SIZE; i++) {
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}
}


void CHIP8::LoadRom(std::string fileName) {
	std::ifstream inputRom(fileName.c_str(), std::ios::binary | std::ios::ate);

	if (inputRom.is_open()) {
		std::streampos romSize = inputRom.tellg();

		char* buffer = new char[romSize];

		inputRom.seekg(0, std::ios::beg);
		inputRom.read(buffer, romSize);
		inputRom.close();

		for (int i = 0; i < romSize; i++) {
			memory[START_ADDRESS + i] = buffer[i];
		}

		delete[] buffer;//free memory
	}
}

void CHIP8::I_00E0() {// clear screen
	for (int i = 0; i < 64 * 32; i++) {
		videoMemory[i] = 0;
	}
}

void CHIP8::I_00EE() {
	--stackPointer;
	programCounter = stack[stackPointer];
}

void CHIP8::I_1NNN() { //jump to address NNN
	uint16_t address = opcode & 0x0FFFu;// mask the upper 4bits
	programCounter = address;
}

void CHIP8::I_2NNN() { // call subroutine at NNN
	uint16_t address = opcode & 0x0FFFu;

	stack[stackPointer] = programCounter;
	++stackPointer;
	programCounter = address;
}

void CHIP8::I_3XNN() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] == byte) {
		programCounter += 2;// the counter is incremented by 2 because size of instruction is 2 bytes
	}
}

void CHIP8::I_4XNN() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if (registers[Vx] != byte) {
		programCounter += 2;
	}
}

void CHIP8::I_5XY0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] == registers[Vy])
	{
		programCounter += 2;
	}
}

void CHIP8::I_6XNN() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = byte;
}

void CHIP8::I_7XNN() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] += byte;
}

void CHIP8::I_8XY0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u)>>4u;

	registers[Vx] = registers[Vy];
}

void CHIP8::I_8XY1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] = registers[Vx] | registers[Vy];
}

void CHIP8::I_8XY2() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] = registers[Vx] & registers[Vy];
}

void CHIP8::I_8XY3() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] = registers[Vx] ^ registers[Vy];
}

void CHIP8::I_8XY4() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = registers[Vx] + registers[Vy];

	if (sum > 255U) {
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[Vx] = sum & 0xFFu;
}

void CHIP8::I_8XY5() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] > registers[Vy])
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[Vx] -= registers[Vy];
}

void CHIP8::I_8XY6() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = (registers[Vx] & 0x1u);

	registers[Vx] >>= 1;
}

void CHIP8::I_8XY7() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vy] > registers[Vx])
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[Vx] = registers[Vy] - registers[Vx];
}

void CHIP8::I_8XYE() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	// Save MSB in VF
	registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

	registers[Vx] <<= 1;
}

void CHIP8::I_9XY0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] != registers[Vy])
	{
		programCounter += 2;
	}
}

void CHIP8::I_ANNN() {
	uint16_t address = (opcode & 0x0FFFu);

	indexRegister = address;
}

void CHIP8::I_BNNN() {
	uint16_t address = (opcode & 0x0FFFu);

	programCounter = address + registers[0];
}

void CHIP8::I_CXNN() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = randByte(randGen) & byte;

}

void CHIP8::I_DXYN() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	 
	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row)
	{
		uint8_t spriteByte = memory[indexRegister + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &videoMemory[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			 
			if (spritePixel)
			{
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					registers[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}