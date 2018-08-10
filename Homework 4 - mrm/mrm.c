#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

int main(int ac, char* argv[]) {
    if (ac != 2) {
        perror("mrm ENTRY\n");
        return 1;
    }
    const char* removeThis = argv[1];
    int result = rmdir(removeThis);
    if (result == -1) {
        //Could be a file. Check if it's a file.
        if (errno == ENOTDIR) {
            int result = unlink(removeThis);
            if (result == -1) {
                perror("Failed to remove file: File does not exist or user lacks sufficient privileges\n");
                return 2;
            }
        }
        //It was a directory. Check if directory failed to open or not.
        else {
            perror("Failed to remove directory: Directory is not empty\n");
            return 3;
        }
    }
    return 0;
}