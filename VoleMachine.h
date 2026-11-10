#ifndef VOLEMACHINE_H
#define VOLEMACHINE_H

#include <string>
#include <cstdint>
#include "Memory.h"
#include "RegisterFile.h"
#include "CPU.h"
#include "ProgramLoader.h"

class VoleMachine {
private:
    Memory memory;
    RegisterFile registers;
    CPU cpu;

public:
    VoleMachine();
    
    bool loadProgramFile(const std::string& filename, int startAddress);
    void runProgram();
    void stepExecution();
    void displayStatus();
    
    // تعديل النوع ليطابق uint8_t
    void setMemoryValue(int address, uint8_t value);
    void setRegisterValue(int regNum, uint8_t value);
    
    Memory& getMemory();
    RegisterFile& getRegisterFile();
    CPU& getCPU();
};

#endif