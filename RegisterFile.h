// RegisterFile.h
#ifndef REGISTERFILE_H
#define REGISTERFILE_H

class RegisterFile {
private:
    int reg[16];
public:
    RegisterFile();
    int readRegister(int index);
    void writeRegister(int index, int value);
    void printRegisters();
};

#endif
