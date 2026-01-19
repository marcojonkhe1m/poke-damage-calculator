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

struct linux_offscreen_buffer {
    void *Memory;
    int Size;
    int Width;
    int Height;
    int Pitch;
    int BytesPerChar = sizeof(chtype);
};

struct color_gradient_info {
    int ColorBase;
    int ColorSteps;
};

// TODO: (marco): This is a global for now
global_variable volatile sig_atomic_t Running;
global_variable volatile sig_atomic_t ResizeRequested;

global_variable linux_offscreen_buffer GlobalBackbuffer;
global_variable color_gradient_info GlobalColorGradientInfo;

internal void InitColors(color_gradient_info *ColorGradientInfo) {
    start_color();
    use_default_colors();

    ColorGradientInfo->ColorBase = 16;
    ColorGradientInfo->ColorSteps = 64;

    if (COLORS < ColorGradientInfo->ColorBase + ColorGradientInfo->ColorSteps) {
        return;
    }

    for (int I = 0; I < ColorGradientInfo->ColorSteps; I++) {

        int Green = 1000 - (I * 1000 / (ColorGradientInfo->ColorSteps - 1));
        int Blue = (I * 1000 / (ColorGradientInfo->ColorSteps - 1));

        init_color(ColorGradientInfo->ColorBase + I, 0, Green, Blue);

        init_pair(
            ColorGradientInfo->ColorBase + I,
            COLOR_BLACK,
            ColorGradientInfo->ColorBase + I);
    }
}

internal void UpdateGradient(color_gradient_info ColorGradientInfo, int BlueOffset, int GreenOffset) {

    for (int i = 0; i < ColorGradientInfo.ColorSteps; i++) {
        int x = i + BlueOffset;
        int y = i + GreenOffset;

        int Blue = (x % ColorGradientInfo.ColorSteps) * 1000 / (ColorGradientInfo.ColorSteps - 1);
        int Green = (y % ColorGradientInfo.ColorSteps) * 1000 / (ColorGradientInfo.ColorSteps - 1);

        init_color(ColorGradientInfo.ColorBase + i, 0, Green, Blue);
    }
}

internal void RenderWeirdGradient(
    linux_offscreen_buffer Buffer,
    color_gradient_info ColorGradientInfo,
    int BlueOffset,
    int GreenOffset) {

    // TODO: (marco) See if it's to pass by value or by reference
    uint8_t *Row = (uint8_t *)Buffer.Memory;
    for (int Y = 0; Y < Buffer.Height; ++Y) {
        chtype *Cell = (chtype *)Row;

        for (int X = 0; X < Buffer.Width; ++X) {
            int StepX = (X + BlueOffset) % ColorGradientInfo.ColorSteps;
            int StepY = (Y + GreenOffset) % ColorGradientInfo.ColorSteps;
            int Step = (StepX + StepY) % ColorGradientInfo.ColorSteps;
            *Cell++ = ' ' | COLOR_PAIR(ColorGradientInfo.ColorBase + Step);
        }

        Row += Buffer.Pitch;
    }
}

internal void LinuxResizeTerminalBuffer(linux_offscreen_buffer *Buffer, int Width, int Height) {

    // TODO: (marco) Bulletproof this
    // maybe not free first but later
    if (Buffer->Memory) {
        munmap(Buffer->Memory, Buffer->Size);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;

    Buffer->Size = Buffer->BytesPerChar * Buffer->Width * Buffer->Height;
    Buffer->Memory = mmap(NULL, Buffer->Size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    Buffer->Pitch = Buffer->Width * Buffer->BytesPerChar;
}

// TODO: (marco) This actually uses nCurses to write to the terminal line by line. line is more optimized then cell I believe. Check this maybe?
internal void LinuxPresentBuffer(linux_offscreen_buffer Buffer, int Width, int Height) {
    erase();
    uint8_t *Row = (uint8_t *)Buffer.Memory;
    for (int y = 0; y < Height; ++y) {
        move(y, 0);
        chtype *Cell = (chtype *)Row;
        addchnstr(Cell, Width);

        Row += Buffer.Pitch;
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

    InitColors(&GlobalColorGradientInfo);

    // TODO:(marco) Take this out and replace with a function, a clamp to ensure valid parameter
    // and a struct possible defined in header file.
    struct winsize WindowSize = {};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize) == 0) {

        resizeterm(WindowSize.ws_row, WindowSize.ws_col);
        LinuxResizeTerminalBuffer(&GlobalBackbuffer, WindowSize.ws_col, WindowSize.ws_row);
        int XOffset = 0;
        int YOffset = 0;

        Running = 1;

        while (Running) {
            if (ResizeRequested) {
                ResizeRequested = 0;

                ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);
                if (WindowSize.ws_row > 0 && WindowSize.ws_col > 0) {
                    resizeterm(WindowSize.ws_row, WindowSize.ws_col);
                    LinuxResizeTerminalBuffer(&GlobalBackbuffer, WindowSize.ws_col, WindowSize.ws_row);
                }
                else {
                    // TODO: (marco) logging
                }
            }

            // TODO: (marco) create UpdateAppAndRender
            RenderWeirdGradient(GlobalBackbuffer, GlobalColorGradientInfo, XOffset, YOffset);
            UpdateGradient(GlobalColorGradientInfo, XOffset, YOffset);
            LinuxPresentBuffer(GlobalBackbuffer, WindowSize.ws_col, WindowSize.ws_row);

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
