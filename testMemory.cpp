#include <iostream>
#include "Memory.h"
using namespace std;

int main() {
    Memory m;

    cout << m.readMemory(0) << endl;        // 0 (الذاكرة فاضية)

    m.writeMemory(0xA3, 5);
    cout << m.readMemory(0xA3) << endl;     // 5

    m.writeMemory(0x10, 0x2A);
    cout << m.readMemory(0x10) << endl;     // 42

    m.writeMemory(300, 1);                  // عنوان غلط
    m.readMemory(-1);                       // عنوان غلط

    m.printMemory();                        // الـ matrix 16x16
}