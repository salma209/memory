#include "ProgramLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>

bool ProgramLoader::loadProgram(const std::string& filename, Memory& memory, int startAddress) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    // Validate start address (Must be even and less than 0xFF)
    if (startAddress < 0 || startAddress >= 255 || startAddress % 2 != 0) {
        std::cerr << "Error: Invalid start address. Must be an even number between 00 and FE." << std::endl;
        return false;
    }

    std::string line;
    int currentAddress = startAddress;

    while (std::getline(file, line)) {
        // Ignore comments starting with ';'
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        std::stringstream ss(line);
        std::string instruction;
        ss >> instruction;

        if (instruction.empty()) continue; // Skip empty lines

        if (instruction.length() < 4) continue;
        instruction = instruction.substr(0, 4); // Take exact 4 hex digits

        if (currentAddress + 1 >= 256) {
            std::cerr << "Error: Memory limit exceeded!" << std::endl;
            return false;
        }

        // Split into 2 bytes
        std::string byte1Str = instruction.substr(0, 2);
        std::string byte2Str = instruction.substr(2, 2);

        uint8_t byte1 = static_cast<uint8_t>(std::stoul(byte1Str, nullptr, 16));
        uint8_t byte2 = static_cast<uint8_t>(std::stoul(byte2Str, nullptr, 16));

        // Using writeMemory matching team's function name
        memory.writeMemory(currentAddress, byte1);
        memory.writeMemory(currentAddress + 1, byte2);

        currentAddress += 2;
    }

    file.close();
    std::cout << "Program loaded successfully starting at address 0x" 
              << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << startAddress << std::dec << std::endl;
    return true;
}
