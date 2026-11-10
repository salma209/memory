#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H

#include <string>
#include "Memory.h"

class ProgramLoader {
public:
    // Loads Vole machine instructions into Memory
    static bool loadProgram(const std::string& filename, Memory& memory, int startAddress);
};

#endif // PROGRAM_LOADER_H
