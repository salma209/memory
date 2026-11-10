#include "VoleMachine.h"
#include <iostream>

VoleMachine::VoleMachine() {}

bool VoleMachine::loadProgramFile(const std::string& filename, int startAddress) {
    if (ProgramLoader::loadProgram(filename, memory, startAddress)) {
        cpu.setPC(startAddress); // Reset/Set Program Counter
        cpu.setHalted(false);
        return true;
    }
    return false;
}

void VoleMachine::runProgram() {
    while (!cpu.isHalted()) {
        cpu.step(memory, registers);
    }
}

void VoleMachine::stepExecution() {
    if (!cpu.isHalted()) {
        cpu.step(memory, registers);
    }
}

void VoleMachine::displayStatus() {
    // يمكنك طباعة حالة الـ CPU والذاكرة هنا أو تركها حسب رغبتك
    std::cout << "PC: " << (int)cpu.getPC() << " | IR: " << cpu.getIR() << std::endl;
}

void VoleMachine::setMemoryValue(int address, uint8_t value) {
    memory.writeMemory(address, value);
}

void VoleMachine::setRegisterValue(int regNum, uint8_t value) {
    // التحويل من رقم الـ Register إلى الحرف المناسب (مثلا 0 -> '0', 1 -> '1', ... 10 -> 'A')
    char regChar = (regNum < 10) ? ('0' + regNum) : ('A' + (regNum - 10));
    registers.writeRegister(regChar, value);
}

Memory& VoleMachine::getMemory() { return memory; }
RegisterFile& VoleMachine::getRegisterFile() { return registers; }
CPU& VoleMachine::getCPU() { return cpu; }