#pragma once

#include <stddef.h>

void *ReadEntireFile(const char *Filename);
void FreeEntireFile(void *FileHandle);

size_t GetTotalBytesInFile(int FileHandle);
