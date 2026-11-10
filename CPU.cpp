#include "CPU.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <cctype>
#include <cstdint>

CPU::CPU() : programCounter(0x00), instructionRegister("0000"), halted(false) {}

int CPU::hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    throw std::invalid_argument("Invalid hex character");
}

std::string CPU::intToHex(int val, int width) {
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(width) << (val & ((1 << (width * 4)) - 1));
    return ss.str();
}

unsigned char CPU::hexStringToByte(const std::string& hex) {
    return (unsigned char)((hexCharToInt(hex[0]) << 4) | hexCharToInt(hex[1]));
}

void CPU::reset() {
    programCounter = 0x00;
    instructionRegister = "0000";
    halted = false;
}

unsigned char CPU::getPC() const { return programCounter; }
void CPU::setPC(unsigned char pc) { programCounter = pc; }
std::string CPU::getIR() const { return instructionRegister; }
void CPU::setHalted(bool h) { halted = h; }
bool CPU::isHalted() const { return halted; }

void CPU::fetch(Memory& memory) {
    if (halted) return;
    uint8_t byte1 = memory.readMemory(programCounter);
    uint8_t byte2 = memory.readMemory(programCounter + 1);
    instructionRegister = intToHex(byte1, 2) + intToHex(byte2, 2);
    programCounter += 2;
}

bool CPU::validate() {
    if (instructionRegister.length() != 4) return false;
    for (char c : instructionRegister) {
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))) {
            return false;
        }
    }
    char opcode = std::toupper(instructionRegister[0]);
    return (opcode >= '1' && opcode <= 'C');
}

void CPU::decodeAndExecute(Memory& memory, RegisterFile& regFile) {
    if (!validate()) {
        halted = true;
        throw std::runtime_error("Error: Invalid instruction detected: " + instructionRegister);
    }

    char opcode = std::toupper(instructionRegister[0]);

    switch (opcode) {
        case '1': { // 1RXY: LOAD R from memory XY
            char r = instructionRegister[1];
            std::string xy = instructionRegister.substr(2, 2);
            regFile.writeRegister(r, memory.readMemory(hexStringToByte(xy)));
            break;
        }
        case '2': { // 2RXY: LOAD R with bit pattern XY
            char r = instructionRegister[1];
            std::string xy = instructionRegister.substr(2, 2);
            regFile.writeRegister(r, hexStringToByte(xy));
            break;
        }
        case '3': { // 3RXY: STORE R into memory XY (or Screen if XY == FF)
            char r = instructionRegister[1];
            std::string xy = instructionRegister.substr(2, 2);
            unsigned char address = hexStringToByte(xy);
            unsigned char val = regFile.readRegister(r);

            memory.writeMemory(address, val);

            // Special handling for Screen Output (Address FF)
            if (std::toupper(xy[0]) == 'F' && std::toupper(xy[1]) == 'F') {
                std::cout << "\n[SCREEN OUTPUT (FF)]: " 
                          << intToHex(val, 2) 
                          << " (Decimal: " << (int)val 
                          << ", Char: " << ((val >= 32 && val <= 126) ? (char)val : '.') << ")\n";
            }
            break;
        }
        case '4': { // 40RS: MOVE R to S
            char r = instructionRegister[2];
            char s = instructionRegister[3];
            regFile.writeRegister(s, regFile.readRegister(r));
            break;
        }
        case '5': { // 5RST: ADD int S and T -> R
            char r = instructionRegister[1];
            char s = instructionRegister[2];
            char t = instructionRegister[3];
            unsigned char valS = regFile.readRegister(s);
            unsigned char valT = regFile.readRegister(t);
            regFile.writeRegister(r, (unsigned char)(valS + valT));
            break;
        }
        case '6': { // 6RST: ADD float S and T -> R
            char r = instructionRegister[1];
            char s = instructionRegister[2];
            char t = instructionRegister[3];
            unsigned char res = addFloatingPoint(regFile.readRegister(s), regFile.readRegister(t));
            regFile.writeRegister(r, res);
            break;
        }
        case '7': { // 7RST: OR S and T -> R
            char r = instructionRegister[1];
            char s = instructionRegister[2];
            char t = instructionRegister[3];
            regFile.writeRegister(r, regFile.readRegister(s) | regFile.readRegister(t));
            break;
        }
        case '8': { // 8RST: AND S and T -> R
            char r = instructionRegister[1];
            char s = instructionRegister[2];
            char t = instructionRegister[3];
            regFile.writeRegister(r, regFile.readRegister(s) & regFile.readRegister(t));
            break;
        }
        case '9': { // 9RST: XOR S and T -> R
            char r = instructionRegister[1];
            char s = instructionRegister[2];
            char t = instructionRegister[3];
            regFile.writeRegister(r, regFile.readRegister(s) ^ regFile.readRegister(t));
            break;
        }
        case 'A': { // A R0X: ROTATE R right X times
            char r = instructionRegister[1];
            int x = hexCharToInt(instructionRegister[3]) % 8;
            unsigned char val = regFile.readRegister(r);
            unsigned char rotated = (val >> x) | (val << (8 - x));
            regFile.writeRegister(r, rotated);
            break;
        }
        case 'B': { // B RXY: JUMP to XY if R == R0
            char r = instructionRegister[1];
            std::string xy = instructionRegister.substr(2, 2);
            if (regFile.readRegister(r) == regFile.readRegister('0')) {
                programCounter = hexStringToByte(xy);
            }
            break;
        }
        case 'C': { // C000: HALT
            halted = true;
            break;
        }
        default:
            halted = true;
            throw std::runtime_error("Unsupported opcode encountered.");
    }
}

bool CPU::step(Memory& memory, RegisterFile& regFile) {
    if (halted) return false;
    fetch(memory);
    decodeAndExecute(memory, regFile);
    return !halted;
}

unsigned char CPU::addFloatingPoint(unsigned char b1, unsigned char b2) {
    auto decode = [](unsigned char b) {
        int sign = (b & 0x80) ? -1 : 1;
        int exp = (b >> 4) & 0x07;
        int mantissa = b & 0x0F;
        return sign * (1.0 + (double)mantissa / 16.0) * std::pow(2.0, exp - 4);
    };

    double sum = decode(b1) + decode(b2);
    if (sum == 0.0) return 0x00;

    unsigned char signBit = (sum < 0) ? 0x80 : 0x00;
    double absSum = std::abs(sum);

    int storedExp = 4;
    while (absSum >= 2.0 && storedExp < 7) {
        absSum /= 2.0;
        storedExp++;
    }
    while (absSum < 1.0 && storedExp > 0) {
        absSum *= 2.0;
        storedExp--;
    }

    if (storedExp < 0) storedExp = 0;
    if (storedExp > 7) {
        storedExp = 7;
        absSum = 1.9375;
    }

    int mantissa = (int)std::round((absSum - 1.0) * 16.0);
    if (mantissa > 15) mantissa = 15;
    if (mantissa < 0) mantissa = 0;

    return signBit | ((storedExp & 0x07) << 4) | (mantissa & 0x0F);
}