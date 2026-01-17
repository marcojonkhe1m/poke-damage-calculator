#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

static void *BufferMemory;
static int BufferWidth;
static int BufferHeight;
static int BufferSize;

static int ColorBase;
static int ColorSteps;

void InitColors() {
    start_color();
    use_default_colors();

    // init_pair(1, COLOR_GREEN, COLOR_BLACK);
    // init_pair(2, COLOR_BLUE, COLOR_BLACK);

    ColorBase = 16;
    ColorSteps = 64;

    if (COLORS < ColorBase + ColorSteps) {
        return;
    }

    for (int I = 0; I < ColorSteps; I++) {

        int Green = 1000 - (I * 1000 / (ColorSteps - 1));
        int Blue = (I * 1000 / (ColorSteps - 1));

        init_color(ColorBase + I, 0, Green, Blue);

        init_pair(ColorBase + I, COLOR_BLACK, ColorBase + I);
    }
}

void UpdateTerminal() {
    clear();

    uint8_t *Row = (uint8_t *)BufferMemory;
    int Pitch = BufferWidth * sizeof(chtype);

    for (int Y = 0; Y < BufferHeight; Y++) {
        chtype *Cells = (chtype *)Row;
        move(Y, 0);
        addchnstr(Cells, BufferWidth);
    }

    Row += Pitch;

    refresh();
}

void ResizeBuffer(int Width, int Height) {

    if (BufferMemory) {
        munmap(BufferMemory, BufferSize);
    }

    BufferWidth = Width;
    BufferHeight = Height;

    BufferSize = BufferWidth * BufferHeight * sizeof(chtype);

    BufferMemory = mmap(NULL, BufferSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void CreateGradient() {
    int Pitch = BufferWidth * sizeof(chtype);
    uint8_t *Row = (uint8_t *)BufferMemory;

    for (int Y = 0; Y < BufferHeight; Y++) {
        chtype *Cell = (chtype *)Row;

        for (int X = 0; X < BufferWidth; X++) {
            int Step = (X * (ColorSteps - 1)) / (BufferWidth - 1);
            Cell[X] = ' ' | COLOR_PAIR(ColorBase + Step);
        }

        Row += Pitch;
    }
}

int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    InitColors();

    int Width;
    int Height;
    getmaxyx(stdscr, Height, Width);

    ResizeBuffer(Width, Height);

    while (1) {
        int Width;
        int Height;
        getmaxyx(stdscr, Height, Width);

        if (Width != BufferWidth || Height != BufferHeight) {
            ResizeBuffer(Width, Height);
        }

        CreateGradient();
        UpdateTerminal();

        if (getch() == 'q')
            break;
    }

    endwin();
    return 0;
}
