#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

// Function to list files and directories in a given path
void ListFilesAndDirectories(const std::string& path, bool recursive) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "Error finding files in the specified path." << std::endl;
        return;
    }

    std::vector<std::string> subdirectories;

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                std::cout << "[DIR] " << findFileData.cFileName << std::endl;
                if (recursive) {
                    subdirectories.push_back(path + "\\" + findFileData.cFileName);
                }
            }
        } else {
            std::cout << "[FILE] " << findFileData.cFileName << std::endl;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    if (recursive) {
        for (const auto& subdir : subdirectories) {
            ListFilesAndDirectories(subdir, true);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path> [/s]" << std::endl;
        return 1;
    }

    std::string path = argv[1];
    bool recursive = (argc >= 3 && std::string(argv[2]) == "/s");

    ListFilesAndDirectories(path, recursive);

    return 0;
}
