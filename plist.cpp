#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

// Function to list all running processes
void ListProcesses() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Error creating snapshot" << std::endl;
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        std::cout << "PID\tProcess Name\t\tParent PID\tParent Process Name" << std::endl;
        do {
            std::cout << pe32.th32ProcessID << "\t" << pe32.szExeFile;

            // Get parent process information
            HANDLE hParentSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hParentSnapshot != INVALID_HANDLE_VALUE) {
                PROCESSENTRY32 peParent;
                peParent.dwSize = sizeof(PROCESSENTRY32);
                if (Process32First(hParentSnapshot, &peParent)) {
                    do {
                        if (peParent.th32ProcessID == pe32.th32ParentProcessID) {
                            std::cout << "\t\t" << peParent.th32ProcessID << "\t\t" << peParent.szExeFile;
                            break;
                        }
                    } while (Process32Next(hParentSnapshot, &peParent));
                }
                CloseHandle(hParentSnapshot);
            }
            std::cout << std::endl;
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
}

int main() {
    ListProcesses();
    return 0; 
}
