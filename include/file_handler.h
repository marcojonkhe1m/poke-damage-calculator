#pragma once

#include <stddef.h>

struct read_file_result {
    size_t ContentsSize;
    void *Contents;
};

read_file_result ReadEntireFile(const char *Filename);
bool WriteEntireFile(const char *Filename, void *Memory, size_t MemorySize);
void FreeEntireFile(void *FileHandle);

size_t GetTotalBytesInFile(int FileHandle);
