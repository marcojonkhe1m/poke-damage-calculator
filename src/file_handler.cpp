#include "file_handler.h"
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void *ReadEntireFile(const char *Filename) {
    void *Result = 0;
    ssize_t BytesRead = 0;

    int FileDescriptor = open(Filename, O_RDONLY);

    if (FileDescriptor != -1) {
        size_t BytesInFile = GetTotalBytesInFile(FileDescriptor);

        if (BytesInFile != 0) {
            Result = std::calloc(1, BytesInFile);

            if (Result) {
                BytesRead = read(FileDescriptor, Result, BytesInFile);

                if (BytesRead == 0) {
                    // NOTE(marco): file read succesfully
                }

                else {
                    // TODO (marco): Logging
                    FreeEntireFile(Result);
                    Result = 0;
                }
            }

            else {
                // TODO (marco): Logging
            }
        }

        else {
            // TODO (marco): Logging
        }
        close(FileDescriptor);
    }

    else {
        // TODO (marco): Logging
    }

    return Result;
}

size_t GetTotalBytesInFile(int FileDescriptor) {
    struct stat StatBuffer = {};
    int Result = 0;
    Result = fstat(FileDescriptor, &StatBuffer);

    if (Result != -1) {
        return (size_t)0;
    }
    return (size_t)StatBuffer.st_size;
}

void FreeEntireFile(void *Memory) {
    if (Memory) {
        free(Memory);
    }
}
