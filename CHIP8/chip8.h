#pragma once
#include <chrono>
#include <random>
#include"constants.h"

class CHIP8 {

	CHIP8() :randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
		randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
	}

public:
	uint8_t registers[16]{}; // 16 8-bit registers
	uint8_t memory[4096]{}; // 4096 bytes of memory (0x0000 0x0FFF)
	
	uint16_t indexRegister{};// memory address for operations, max memory address is too big for 8bit register(max =65535)

	uint16_t programCounter{};// keep track of which instruction to execute next
	uint16_t stack[16]{};//keep track of order of execution
	uint8_t stackPointer{}; // 1-byte stack pointer because it will only use stack indexes

	uint8_t delayTimer{};
	uint8_t soundTimer{};

	uint8_t keypad[16]{}; //16 characters
	uint32_t videoMemory[64 * 32]{};
	uint16_t opcode;

	std::default_random_engine randGen;
	std::uniform_int_distribution<uint8_t> randByte;

	void LoadRom(std::string fileName);// no const char* c style shit

	//instructions
	//NNN refers to address
	//NN to byte
	//N to nibble
	//X and Y to registers
	void I_00E0();
	void I_00EE();
	void I_1NNN();
	void I_2NNN();
	void I_3XNN();
	void I_4XNN();
	void I_5XY0();
	void I_6XNN();
	void I_7XNN();
	void I_8XY0();
	void I_8XY1();
	void I_8XY2();
	void I_8XY3();
	void I_8XY4();
	void I_8XY5();
	void I_8XY6();
	void I_8XY7();
	void I_8XYE();
	void I_8XYE();
	void I_9XY0();
	void I_ANNN();
	void I_BNNN();
	void I_CXNN();
	void I_DXYN();
	void I_EX9E();
	void I_EXA1();
	void I_FX07();
	void I_FX0A();
	void I_FX15();
	void I_FX18();
	void I_FX1E();
	void I_FX29();
	void I_FX33();
	void I_FX55();
	void I_FX65();
};