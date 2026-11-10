#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <string>
#include "Memory.h"
#include "RegisterFile.h"

class CPU {
private:
    unsigned char programCounter;    
    std::string instructionRegister; 
    bool halted;

    
    int hexCharToInt(char c);
    std::string intToHex(int val, int width = 2);
    unsigned char hexStringToByte(const std::string& hex);
    unsigned char addFloatingPoint(unsigned char b1, unsigned char b2);

public:
    CPU();

    
    void fetch(Memory& memory);
    bool validate();
    void decodeAndExecute(Memory& memory, RegisterFile& regFile);
    bool step(Memory& memory, RegisterFile& regFile); 


    unsigned char getPC() const;
    void setPC(unsigned char pc);
    std::string getIR() const;
    void setHalted(bool h);
    bool isHalted() const;
    void reset();
};

#endif 