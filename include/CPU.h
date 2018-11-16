#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include "Stack.h"

typedef unsigned short u16;

class CPU{
private:
	std::vector<u16> memory;
	std::vector<u16> registers;
	jci::Stack<u16> stack;
	u16 programCounter;
	u16 OPcode;
	std::string tempInput;
	bool halt;
	void execute();
	u16 decodeArgumentOut(u16);
	void decodeArgumentIn(u16, u16);
public:
	CPU();
	void setMemory(u16, u16);
	void tick();	
};