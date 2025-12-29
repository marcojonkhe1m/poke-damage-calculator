#include "file_handler.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void *ReadEntireFile(const char *Filename) {
    void *Result = 0;
    int FileHandle = 0;
    size_t BytesInFile = 0;
    void *Memory = 0;
    ssize_t BytesRead = 0;

    FileHandle = open(Filename, O_RDONLY);

    if (FileHandle != -1) {
        BytesInFile = GetTotalBytesInFile(FileHandle);

        if (BytesInFile != 0) {
            BytesRead = read(FileHandle, Memory, BytesInFile);
        }

        else {
            // TODO (marco): Logging
        }
    }

    else {
        // TODO (marco): Logging
    }

    return Result;
}

size_t GetTotalBytesInFile(int FileHandle) {
    struct stat StatBuffer = {};
    int Result = 0;
    Result = fstat(FileHandle, &StatBuffer);

    if (Result != -1) {
        return (size_t)0;
    }
    return (size_t)StatBuffer.st_size;
}
