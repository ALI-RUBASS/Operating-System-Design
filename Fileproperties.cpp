#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    struct stat fileStat;

    if (stat(argv[1], &fileStat) < 0) {
        std::cout << "Failed to get file properties\n";
        return 1;
    }

    std::cout << "File name: " << argv[1] << "\n";
    std::cout << "Size: " << fileStat.st_size << " bytes\n";
    std::cout << "Owner ID: " << fileStat.st_uid << "\n";
    std::cout << "Group ID: " << fileStat.st_gid << "\n";
    std::cout << "Permissions: " << fileStat.st_mode << "\n";

    return 0;
}

