   #include "Memory.h"
   #include <iostream>
   #include <iomanip>
   using namespace std;

Memory::Memory() {
    for (int i = 0; i < 256; i++) {
        mem[i] = 0;
    }
}

int Memory::readMemory(int index) {
    if (index < 0 || index >= 256) {
        std::cout << "Invalid memory address" << std::endl;
        return 0;
    }
    return mem[index];
}

void Memory::writeMemory(int index, int value) {
    if (index < 0 || index >= 256) {
        std::cout << "Invalid memory address" << std::endl;
    } else {
        mem[index] = value & 0xFF;
    }
}

void Memory::printMemory() {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
                std::cout << std::hex << std::setw(2) << std::setfill ('0') << mem [i * 16 + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::dec << std::setfill(' ');
}