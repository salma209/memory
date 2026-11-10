// Memory.h
#ifndef MEMORY_H
#define MEMORY_H

class Memory {
private:
    int mem[256];
public:
    Memory();
    int readMemory(int index);
    void writeMemory(int index, int value);
    void printMemory();
};

#endif