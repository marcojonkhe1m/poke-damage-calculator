#include "file_handler.h"
#include <fcntl.h>

void *ReadEntireFile(const char *Filename) {
    void *Result = 0;

    int FileHandle = open(Filename, O_RDONLY);
};
