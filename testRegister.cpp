#include <iostream>
#include "RegisterFile.h"
using namespace std;

int main() {
    RegisterFile r;

    cout << r.readRegister(0) << endl;       

    r.writeRegister(5, 0x2A);
    cout << r.readRegister(5) << endl;       

    r.writeRegister(15, 300);                
    cout << r.readRegister(15) << endl;      

    r.writeRegister(16, 1);                  
    r.readRegister(-1);                      

    r.printRegisters();                      
}