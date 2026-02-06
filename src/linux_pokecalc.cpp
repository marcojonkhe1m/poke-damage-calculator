/* TODO: (marco) This is not the final platform layer!!!

   - saves
   - asset loading
   - streak!!
   - more streak
*/

#define internal static
#define local_persist static
#define global_variable static

#include <stdint.h>

global_variable int GlobalColorBase = 16;
global_variable const int GlobalColorSteps = 64;

#include "pokecalc.cpp"
#include "pokecalc.h"

#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "linux_pokecalc.h"

// TODO: (marco): This is a global for now
global_variable volatile sig_atomic_t GlobalRunning;
global_variable volatile sig_atomic_t GlobalResizeRequested;

global_variable linux_offscreen_buffer GlobalBackbuffer;
global_variable linux_color_gradient_info GlobalColorGradientInfo;

static inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
};

internal void LinuxInitColors(linux_color_gradient_info *ColorGradientInfo) {
    start_color();
    use_default_colors();

    ColorGradientInfo->ColorBase = GlobalColorBase;
    ColorGradientInfo->ColorSteps = GlobalColorSteps;
    int Base = GlobalColorBase;
    int Steps = GlobalColorSteps;

    if (COLORS < Base + Steps) {
        return;
    }

    for (int i = 0; i < Steps; i++) {
        int Blue = 1000 - (i * 1000 / (Steps - 1));
        int Green = (i * 1000 / (Steps - 1));

        init_color(Base + i, 0, Green, Blue);

        init_pair(
            Base + i,
            COLOR_BLACK,
            Base + i);
    }
}

internal void LinuxUpdateGradient(color_gradient_info *ColorGradientInfo) {
    int Base = ColorGradientInfo->ColorBase;
    int Steps = ColorGradientInfo->ColorSteps;

    for (int i = 0; i < Steps; i++) {
        int Blue = 1000 - (ColorGradientInfo->Blue[i] * 1000 / (Steps - 1));
        int Green = (ColorGradientInfo->Green[i] * 1000 / (Steps - 1));

        init_color(Base + i, 0, Green, Blue);

        init_pair(
            Base + i,
            COLOR_BLACK,
            Base + i);
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
    int BytesPerChar = sizeof(chtype);

    Buffer->Size = BytesPerChar * Buffer->Width * Buffer->Height;
    Buffer->Memory = mmap(NULL, Buffer->Size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    Buffer->Pitch = Buffer->Width * BytesPerChar;
}

// TODO: (marco) This actually uses nCurses to write to the terminal line by line. line is more optimized then cell I believe. Check this maybe?
internal void LinuxPresentBuffer(linux_offscreen_buffer *Buffer, int Width, int Height) {
    erase();
    uint8_t *BufferRow = (uint8_t *)Buffer->Memory;
    chtype TargetRow[Width];

    for (int y = 0; y < Height; ++y) {
        uint32_t *Cell = (uint32_t *)BufferRow;

        for (int x = 0; x < Width; ++x) {
            char Character = (uint8_t)(*Cell & 0xFF);
            uint8_t ForegroundColor = (uint8_t)((*Cell >> 8) & 0xFF);

            TargetRow[x] = Character | COLOR_PAIR(ForegroundColor);
            Cell++;
        }
        mvaddchnstr(y, 0, TargetRow, Width);

        BufferRow += Buffer->Pitch;
    }

    refresh();
}

internal void
LinuxSignalHandler(int Signo, siginfo_t *Info, void *Context) {
    switch (Signo) {
    case SIGWINCH:
        GlobalResizeRequested = 1;
        break;
    case SIGINT:
    case SIGHUP:
    case SIGTERM:
        // TODO: (marco) Handle this with a message to the user, maybe differntly per type?
        GlobalRunning = 0;
        break;
    }
}

int main() {

    struct sigaction Sa = {};
    Sa.sa_sigaction = LinuxSignalHandler;
    Sa.sa_flags = SA_SIGINFO;

    int KeyPressed;

    sigaction(SIGINT, &Sa, NULL);
    sigaction(SIGTERM, &Sa, NULL);
    sigaction(SIGHUP, &Sa, NULL);
    sigaction(SIGWINCH, &Sa, NULL);

    ESCDELAY = 25;
    initscr();
    raw();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);

    LinuxInitColors(&GlobalColorGradientInfo);
    // TODO:(marco) Take this out and replace with a function, a clamp to ensure valid parameter
    // and a struct possible defined in header file.
    struct winsize WindowSize
        = {};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize) == 0) {

        resizeterm(WindowSize.ws_row, WindowSize.ws_col);
        LinuxResizeTerminalBuffer(&GlobalBackbuffer, WindowSize.ws_col, WindowSize.ws_row);

        GlobalRunning = 1;

        uint64_t LastCycleCount = rdtsc();
        struct timespec LastTime = {};
        clock_gettime(CLOCK_MONOTONIC, &LastTime);
        while (GlobalRunning) {
            if (GlobalResizeRequested) {
                GlobalResizeRequested = 0;

                ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);
                if (WindowSize.ws_row > 0 && WindowSize.ws_col > 0) {
                    resizeterm(WindowSize.ws_row, WindowSize.ws_col);
                    LinuxResizeTerminalBuffer(&GlobalBackbuffer, WindowSize.ws_col, WindowSize.ws_row);
                }
                else {
                    // TODO: (marco) logging
                }
            }

            KeyPressed = getch();

            if (KeyPressed == 'q') {
                GlobalRunning = 0;
            }
            else if (KeyPressed == KEY_UP) {
            }
            else if (KeyPressed == KEY_DOWN) {
            }
            else if (KeyPressed == KEY_LEFT) {
            }
            else if (KeyPressed == KEY_RIGHT) {
            }
            else if (KeyPressed == KEY_ENTER) {
            }
            else if (KeyPressed == KEY_BACKSPACE) {
            }
            else if (KeyPressed == KEY_F(1)) {
            }
            else if (KeyPressed == 27) {
                nodelay(stdscr, TRUE);

                int next = getch();
                if (next == ERR) {
                    // NOTE: (marco) real esc is pressed - handled below
                }

                else {
                    ungetch(next);
                    ungetch(27);
                }
            }

            // TODO: (marco) create UpdateAppAndRender
            app_offscreen_buffer Buffer = {};
            Buffer.Memory = GlobalBackbuffer.Memory;
            Buffer.Width = GlobalBackbuffer.Width;
            Buffer.Height = GlobalBackbuffer.Height;
            Buffer.Pitch = GlobalBackbuffer.Pitch;
            color_gradient_info ColorInfo = {};
            ColorInfo.ColorBase = GlobalColorGradientInfo.ColorBase;
            ColorInfo.ColorSteps = GlobalColorGradientInfo.ColorSteps;

            AppUpdateAndRender(&Buffer, &ColorInfo);
            LinuxUpdateGradient(&ColorInfo);
            LinuxPresentBuffer(&GlobalBackbuffer, WindowSize.ws_col, WindowSize.ws_row);

            napms(100);

            uint64_t EndCycleCount = rdtsc();
            struct timespec EndTime = {};
            clock_gettime(CLOCK_MONOTONIC, &EndTime);

            // TODO: (marco) display the value here
            uint64_t CyclesElapsed = EndCycleCount - LastCycleCount;
            int64_t LastTimeNs = LastTime.tv_sec * 1000000000LL + LastTime.tv_nsec;
            int64_t EndTimeNs = EndTime.tv_sec * 1000000000LL + EndTime.tv_nsec;
            int64_t NsPerFrame = EndTimeNs - LastTimeNs;
            float MsPerFrame = (float)NsPerFrame / 1000000.0f;
            float FPS = 1000.0f / MsPerFrame;
            float MCPF = ((float)CyclesElapsed / (1000.0f * 1000.0f));

#if 0
            mvprintw(0, 0, "%.02fms/f\n", MsPerFrame);
            mvprintw(1, 0, "%.02ff/s\n", FPS);
            mvprintw(2, 0, "%.02fmc/f\n", MCPF);
            refresh();
#endif
            LastCycleCount = EndCycleCount;
            LastTime = EndTime;
        }
    }
    else {
        // TODO:(marco) Logging
    }
    endwin();
    return 0;
}
