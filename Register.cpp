#include <iostream>
#include <iomanip>
using namespace std;

class RegisterFile {
private:
    int reg[16];
public:
    RegisterFile() {
        for (int i = 0; i < 16; i++) {
            reg[i] = 0;
        }
    }
    int readRegister(int index) {
        if (index < 0 || index >= 16) {
            cout << "Invalid register address" << endl;
            return 0;
        }
        return reg[index];
    }
    void writeRegister(int index, int value) {
        if (index < 0 || index >= 16) {
            cout << "Invalid register address" << endl;
        } else {
            reg[index] = value & 0xFF; //  بتخزن القيمة في الريجستر بعد ما تتأكد انها 8 بت
        }
    }
    void printRegisters() {
        for (int i = 0; i < 16; i++) {
            cout << "R" << i << ": " << hex << setw(2) << setfill('0') << reg[i] << " ";
        }
        cout << endl;
        cout << dec;
    }
};
int main() {
    RegisterFile m;
    cout << m.readRegister(0) << endl; 
    m.writeRegister(0xA3, 5);
    cout << m.readRegister(0xA3) << endl; 
    m.printRegisters();
    m.writeRegister(5, 0x2A);
    cout << m.readRegister(5) << endl;   // هتطبع 42 عشري
    m.printRegisters();
}
