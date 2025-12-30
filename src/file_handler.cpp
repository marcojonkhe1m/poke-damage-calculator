#include "file_handler.h"
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

read_file_result ReadEntireFile(const char *Filename) {
    read_file_result Result = {};
    int FileDescriptor = open(Filename, O_RDONLY);

    if (FileDescriptor != -1) {
        size_t BytesInFile = GetTotalBytesInFile(FileDescriptor);

        if (BytesInFile != 0) {
            Result.Contents = std::calloc(1, BytesInFile);

            if (Result.Contents) {
                ssize_t BytesRead = read(FileDescriptor, Result.Contents, BytesInFile);

                if (BytesRead == 0) {
                    // NOTE(marco): file read succesfully
                    Result.ContentsSize = BytesInFile;
                }

                else {
                    // TODO (marco): Logging
                    FreeEntireFile(Result.Contents);
                    Result.Contents = 0;
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

bool WriteEntireFile(const char *Filename, void *Memory, size_t MemorySize) {
    bool Result = false;
    int FileDescriptor = creat(Filename, S_IRWXU);

    if (FileDescriptor != -1) {
        ssize_t BytesWritten = write(FileDescriptor, Memory, MemorySize);

        if (BytesWritten != -1) {
            // NOTE(marco): Write succesfull!
            Result = (BytesWritten == MemorySize);
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

void FreeEntireFile(void *Memory) {
    if (Memory) {
        free(Memory);
    }
}
