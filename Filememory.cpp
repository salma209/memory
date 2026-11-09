#include<iostream>
#include<iomanip>
using namespace std;

class Memory{
    private:
    int mem[256];
    public:
        Memory() {
            for (int i = 0; i < 256; i++) {
                mem[i] = 0;
            }
        };
        int readMemory(int index) {
            if (index < 0 || index >= 256) {
                cout << "Invalid memory address" << endl;
            return 0;
            }
            else{
                return mem[index];
            }
            
        };
        void writeMemory(int index, int value){
            if(index < 0 || index >= 256){
                cout << "Invalid memory address" << endl;
            }
            else{
                mem[index] = value;
                }
            }
        void printMemory(){
            for(int row = 0; row < 16; row++){
                for(int col = 0; col < 16; col++){
                    cout << hex << setw(2) << setfill('0') << mem[row * 16 + col] << " ";
                }
                cout << endl;
            }
        };
};
int main() {
    Memory m;
    cout << m.readMemory(0) << endl;   // 0
    m.writeMemory(0xA3, 5);
    cout << m.readMemory(0xA3) << endl; // 5
    m.printMemory();
}