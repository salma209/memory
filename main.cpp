// main.cpp
#include <iostream>
#include "RegisterFile.h"
using namespace std;

int main() {
    RegisterFile r;
    r.writeRegister(0, 5);
    cout << r.readRegister(0) << endl;
    r.printRegisters();
}