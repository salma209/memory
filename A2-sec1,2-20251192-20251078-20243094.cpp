#include <iostream>
#include <iomanip>
#include "VoleMachine.h"

using namespace std;

// دالة لعرض محتويات الذاكرة كـ Matrix تماماً كما طلب في الأسلايمنت
void displayMemoryMatrix(Memory& memory) {
    cout << "\n==================== MAIN MEMORY ====================\n     ";
    for (int col = 0; col < 16; ++col) {
        cout << hex << uppercase << col << " ";
    }
    cout << "\n-----------------------------------------------------\n";

    for (int row = 0; row < 16; ++row) {
        cout << hex << uppercase << row << " | ";
        for (int col = 0; col < 16; ++col) {
            int address = (row << 4) | col;
            cout << setw(2) << setfill('0') << hex << uppercase 
                 << (int)memory.readMemory(address) << " ";
        }
        cout << "\n";
    }
    cout << "=====================================================\n";
}

// دالة لعرض حالة الـ CPU والـ Registers
void displayCPUStatus(CPU& cpu, RegisterFile& regFile) {
    cout << "\n------------------- CPU STATUS -------------------\n";
    for (int i = 0; i < 16; ++i) {
        char regChar = (i < 10) ? ('0' + i) : ('A' + (i - 10));
        cout << "R" << regChar << ": " << setw(2) << setfill('0') << hex << uppercase 
             << (int)regFile.readRegister(regChar) << "   ";
        if ((i + 1) % 4 == 0) cout << "\n";
    }
    cout << "PC: " << setw(2) << setfill('0') << hex << uppercase << (int)cpu.getPC() << "   ";
    cout << "IR: " << cpu.getIR() << "\n";
    cout << "--------------------------------------------------\n";
}

int main() {
    VoleMachine vole;
    int choice;
    string filename;
    int startAddress;

    do {
        cout << "\n================ Vole Machine Simulator ================\n";
        cout << "1. Load Program from File\n";
        cout << "2. Run Program at Once\n";
        cout << "3. Run Program Step by Step\n";
        cout << "4. Display Memory & CPU Status\n";
        cout << "5. Overwrite Register or Memory Cell\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter program filename (e.g., program.txt): ";
                cin >> filename;
                cout << "Enter start address in hex (e.g., 00, 02, 04): ";
                cin >> hex >> startAddress;

                if (vole.loadProgramFile(filename, startAddress)) {
                    cout << "[SUCCESS] Program loaded successfully at address 0x" 
                         << hex << uppercase << startAddress << "!\n";
                } else {
                    cout << "[ERROR] Failed to load program file. Check file path.\n";
                }
                break;
            }
            case 2: {
                cout << "[INFO] Running program at once...\n";
                vole.getCPU().setHalted(false);
                vole.runProgram();
                cout << "[INFO] Program execution finished.\n";
                displayMemoryMatrix(vole.getMemory());
                displayCPUStatus(vole.getCPU(), vole.getRegisterFile());
                break;
            }
            case 3: {
                cout << "[INFO] Running program step by step. Press Enter after each step.\n";
                vole.getCPU().setHalted(false);
                cin.ignore(); // Clear buffer
                while (!vole.getCPU().isHalted()) {
                    displayMemoryMatrix(vole.getMemory());
                    displayCPUStatus(vole.getCPU(), vole.getRegisterFile());
                    cout << "Press [Enter] to execute next step (or type 'q' then Enter to stop): ";
                    string input;
                    getline(cin, input);
                    if (input == "q" || input == "Q") break;

                    vole.stepExecution();
                }
                cout << "[INFO] Execution ended or stopped.\n";
                displayMemoryMatrix(vole.getMemory());
                displayCPUStatus(vole.getCPU(), vole.getRegisterFile());
                break;
            }
            case 4: {
                displayMemoryMatrix(vole.getMemory());
                displayCPUStatus(vole.getCPU(), vole.getRegisterFile());
                break;
            }
            case 5: {
                int type;
                cout << "1. Overwrite Memory Cell\n2. Overwrite Register\nEnter choice: ";
                cin >> type;
                if (type == 1) {
                    int addr, val;
                    cout << "Enter memory address (hex): ";
                    cin >> hex >> addr;
                    cout << "Enter new value (hex): ";
                    cin >> hex >> val;
                    vole.setMemoryValue(addr, (uint8_t)val);
                    cout << "[SUCCESS] Memory updated.\n";
                } else if (type == 2) {
                    char reg;
                    int val;
                    cout << "Enter register name (0-F): ";
                    cin >> reg;
                    cout << "Enter new value (hex): ";
                    cin >> hex >> val;
                    vole.getRegisterFile().writeRegister(toupper(reg), (uint8_t)val);
                    cout << "[SUCCESS] Register updated.\n";
                }
                break;
            }
            case 6:
                cout << "Exiting Vole Machine Simulator. Goodbye!\n";
                break;
            default:
                cout << "[ERROR] Invalid choice. Try again.\n";
        }
    } while (choice != 6);

    return 0;
}