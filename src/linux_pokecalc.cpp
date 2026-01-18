#include <ncurses.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#define internal static
#define local_persist static
#define global_variable static

// Unfortunately a comment to keep the streak
// TODO: (marco): This is a global for now
global_variable volatile sig_atomic_t Running;
global_variable volatile sig_atomic_t ResizeRequested;

global_variable void *BufferMemory;
global_variable int BufferSize;
global_variable int BufferWidth;
global_variable int BufferHeight;
global_variable int BytesPerChar = 4;

global_variable int ColorBase;
global_variable int ColorSteps;

internal void InitColors() {
    start_color();
    use_default_colors();

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

internal void UpdateGradient(int BlueOffset, int GreenOffset) {

    for (int i = 0; i < ColorSteps; i++) {
        int x = i + BlueOffset;
        int y = i + GreenOffset;

        int Blue = (x % ColorSteps) * 1000 / (ColorSteps - 1);
        int Green = (y % ColorSteps) * 1000 / (ColorSteps - 1);

        init_color(ColorBase + i, 0, Green, Blue);
    }
}

internal void RenderWeirdGradient(int BlueOffset, int GreenOffset) {
    int Width = BufferWidth;
    int Height = BufferHeight;

    int Pitch = Width * BytesPerChar;
    uint8_t *Row = (uint8_t *)BufferMemory;
    for (int Y = 0; Y < Height; ++Y) {
        chtype *Cell = (chtype *)Row;

        for (int X = 0; X < Width; ++X) {
            int StepX = (X + BlueOffset) % ColorSteps;
            int StepY = (Y + GreenOffset) % ColorSteps;
            int Step = (StepX + StepY) % ColorSteps;
            *Cell++ = ' ' | COLOR_PAIR(ColorBase + Step);
        }

        Row += Pitch;
    }
}

internal void LinuxResizeTerminalBuffer(int Width, int Height) {

    // TODO: (marco) Bulletproof this
    // maybe not free first but later
    if (BufferMemory) {
        munmap(BufferMemory, BufferSize);
    }

    BufferWidth = Width;
    BufferHeight = Height;

    BufferSize = BytesPerChar * BufferWidth * BufferHeight;
    BufferMemory = mmap(NULL, BufferSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

// TODO: (marco) This actually uses nCurses to write to the terminal line by line. line is more optimized then cell I believe. Check this maybe?
internal void LinuxPresentBuffer(int Width, int Height) {
    erase();
    int Pitch = Width * BytesPerChar;
    uint8_t *Row = (uint8_t *)BufferMemory;
    for (int y = 0; y < Height; ++y) {
        move(y, 0);
        chtype *Cell = (chtype *)Row;
        addchnstr(Cell, Width);

        Row += Pitch;
    }

    refresh();
}

internal void
LinuxSignalHandler(int Signo, siginfo_t *Info, void *Context) {
    switch (Signo) {
    case SIGWINCH:
        ResizeRequested = 1;
        break;
    case SIGINT:
    case SIGHUP:
    case SIGTERM:
        // TODO: (marco) Handle this with a message to the user, maybe differntly per type?
        Running = 0;
        break;
    }
}

int main() {

    struct sigaction Sa = {};
    Sa.sa_sigaction = LinuxSignalHandler;
    Sa.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, &Sa, NULL);
    sigaction(SIGTERM, &Sa, NULL);
    sigaction(SIGHUP, &Sa, NULL);
    sigaction(SIGWINCH, &Sa, NULL);

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);

    InitColors();

    // TODO:(marco) Take this out and replace with a function, a clamp to ensure valid parameter
    // and a struct possible defined in header file.
    struct winsize WindowSize = {};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize) == 0) {

        resizeterm(WindowSize.ws_row, WindowSize.ws_col);
        LinuxResizeTerminalBuffer(WindowSize.ws_col, WindowSize.ws_row);
        int XOffset = 0;
        int YOffset = 0;
        // clearok(stdscr, TRUE);
        // refresh();

        Running = 1;

        while (Running) {
            if (ResizeRequested) {
                ResizeRequested = 0;

                ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);
                if (WindowSize.ws_row > 0 && WindowSize.ws_col > 0) {
                    resizeterm(WindowSize.ws_row, WindowSize.ws_col);
                    LinuxResizeTerminalBuffer(WindowSize.ws_col, WindowSize.ws_row);
                }
                else {
                    // TODO: (marco) logging
                }
            }

            // TODO: (marco) create UpdateAppAndRender
            RenderWeirdGradient(XOffset, YOffset);
            UpdateGradient(XOffset, YOffset);
            LinuxPresentBuffer(WindowSize.ws_col, WindowSize.ws_row);

            ++XOffset;
            YOffset += 2;

            napms(100);

            if (getch() == 'q') {
                Running = 0;
            }
        }
    }
    else {
        // TODO:(marco) Logging
    }
    endwin();
    return 0;
}
