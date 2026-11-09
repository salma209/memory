   #include "RegisterFile.h"
   #include <iostream>
   #include <iomanip>
   using namespace std;

RegisterFile::RegisterFile() {
    for (int i = 0; i < 16; i++) {
        reg[i] = 0;
    }
}

int RegisterFile::readRegister(int index) {
    if (index < 0 || index >= 16) {
        std::cout << "Invalid register address" << std::endl;
        return 0;
    }
    return reg[index];
}

void RegisterFile::writeRegister(int index, int value) {
    if (index < 0 || index >= 16) {
        std::cout << "Invalid register address" << std::endl;
    } else {
        reg[index] = value & 0xFF;
    }
}

void RegisterFile::printRegisters() {
    for (int i = 0; i < 16; i++) {
        std::cout << "R" << i << ": " << std::hex << std::setw(2) << std::setfill('0') << reg[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::dec;
    std::cout << std::dec << std::setfill(' ');
}