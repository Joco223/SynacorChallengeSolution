#include <fstream>
#include <iterator>
#include <vector>
#include <iostream>

#include "CPU.h"

typedef unsigned short u16;

int main() {

	CPU cpu;

	std::ifstream input("challenge.bin", std::ios::binary );
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

	for(int i = 0; i < buffer.size(); i += 2) {
		cpu.setMemory(i/2, buffer[i+1] << 8 | buffer[i] & 0xFF);
	}

	std::cerr << "Finished loading the program.\n";

	cpu.tick();
	
	return 0;
}