#include "CPU.h"

CPU::CPU() 
	:
	halt(false),
	programCounter(0),
	OPcode(0)
{
	memory.resize(32767);
	registers.resize(8);
	tempInput = "";
}

void CPU::setMemory(u16 pos, u16 value) {
	memory[pos] = value;
}

u16 CPU::decodeArgumentOut(u16 value) {
	if(value < 32768) {
		return value;
	}else if(value < 32776) {
		return registers[value - 32768];
	}else{
		throw std::invalid_argument("Received invalid memory address.");
	}
}

void CPU::decodeArgumentIn(u16 position, u16 value) {
	if(position < 32768) {
		memory[position] = value;
	}else if(position < 32776) {
		registers[position - 32768] = value;
	}else{
		throw std::invalid_argument("Received invalid memory address.");
	}
}


void CPU::execute() {
	u16 instruction = OPcode;
	u16 tempMemLoc = 0;
	int mask = 0b0111111111111111;
	int result = 0;
	std::string tmp = "";
	std::string wut = "";
	u16 arg1 = memory[programCounter + 1];
	u16 arg2 = memory[programCounter + 2];
	u16 arg3 = memory[programCounter + 3];
	switch(instruction) {
		case 0: //halt: 0
			halt = true;
			break;
		case 1: //set: 1 a b
			registers[arg1 - 32768] = decodeArgumentOut(arg2);
			programCounter += 3;
			break;
		case 2: //push: 2 a
			stack.push(decodeArgumentOut(arg1));
			programCounter += 2;
			break;
		case 3: //pop: 3 a
			if(!stack.isEmpty()) {
				decodeArgumentIn(arg1, stack.peek());
				stack.pop();
				programCounter += 2;
			}else{
				throw std::invalid_argument("Tried to pop when the stack is empty.");
			}
			break;
		case 4: //eq: 4 a b c
			if(decodeArgumentOut(arg2) == decodeArgumentOut(arg3)) {
				decodeArgumentIn(arg1, 1);
			}else{
				decodeArgumentIn(arg1, 0);
			}
			programCounter += 4;
			break;
		case 5: //gt: 5 a b c
			if(decodeArgumentOut(arg2) > decodeArgumentOut(arg3)) {
				decodeArgumentIn(arg1, 1);
			}else{
				decodeArgumentIn(arg1, 0);
			}
			programCounter += 4;
			break;
		case 6: //jmp: 6 a
			programCounter = decodeArgumentOut(arg1);
			break;
		case 7: //jt: 7 a b
			if(decodeArgumentOut(arg1) != 0) {programCounter = decodeArgumentOut(arg2);}else{programCounter += 3;}
			break;
		case 8: //jf: 8 a b
			if(decodeArgumentOut(arg1) == 0) {programCounter = decodeArgumentOut(arg2);}else{programCounter += 3;}	
			break;
		case 9: //add: 9 a b c
			decodeArgumentIn(arg1, (decodeArgumentOut(arg2) + decodeArgumentOut(arg3)) % 32768);
			programCounter += 4;
			break;
		case 10: //mult: 10 a b c
			decodeArgumentIn(arg1, (decodeArgumentOut(arg2) * decodeArgumentOut(arg3)) % 32768);
			programCounter += 4;
			break;
		case 11: //mod: 11 a b c
			decodeArgumentIn(arg1, decodeArgumentOut(arg2) % decodeArgumentOut(arg3));
			programCounter += 4;
			break;
		case 12: //and: 12 a b c
			decodeArgumentIn(arg1, decodeArgumentOut(arg2) & decodeArgumentOut(arg3));
			programCounter += 4;
			break;
		case 13: //or: 13 a b c
			decodeArgumentIn(arg1, decodeArgumentOut(arg2) | decodeArgumentOut(arg3));
			programCounter += 4;
			break;
		case 14: //not: 14 a b
			result = decodeArgumentOut(arg2) ^ mask;
			decodeArgumentIn(arg1, result);
			programCounter += 3;
			break;
		case 15: //rmem: 15 a b
			decodeArgumentIn(arg1, memory[decodeArgumentOut(arg2)]);
			programCounter += 3;
			break;
		case 16: //wmem: 16 a b
			memory[decodeArgumentOut(arg1)] = decodeArgumentOut(arg2);
			programCounter += 3;
			break;
		case 17: //call: 17 a
			tempMemLoc = programCounter + 2;
			stack.push(tempMemLoc);
			programCounter = decodeArgumentOut(arg1);
			break;
		case 18: //ret: 18
			if(stack.isEmpty()) {
				halt = true;
			}else{
				programCounter = stack.peek();
				stack.pop();
			}
			break;
		case 19: //out: 19 a
			std::cout << static_cast<char>(decodeArgumentOut(arg1));
			programCounter += 2;
			break;
		case 20: //in: 20 a
			if(tempInput == "") {
				std::getline(std::cin, tmp);
				tempInput = tempInput + tmp + '\n';
				decodeArgumentIn(arg1, (int)tempInput[0]);
				tempInput.erase(0, 1);
				programCounter += 2;
			}else{
				decodeArgumentIn(arg1, (int)tempInput[0]);
				tempInput.erase(0, 1);
				programCounter += 2;
			}		
			break;
		case 21: //noop: 21
			programCounter++;
			break;
		default:
			std::cerr << "Unknown instruction: " << OPcode << '\n';
			halt = true;
			break;
	}
}

void CPU::tick() {
	while(!halt) {
		OPcode = memory[programCounter];
		execute();
	}
	std::cerr << "The CPU has halted.\n";
}