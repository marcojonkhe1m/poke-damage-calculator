#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define internal static
#define local_persist static
#define global_variable static

struct terminal_buffer {
    int Width;
    int Height;
    chtype *Cells;
};

// TODO: (marco): This is a global for now
global_variable volatile sig_atomic_t Running;
global_variable volatile sig_atomic_t ResizeRequested;

global_variable terminal_buffer TerminalBuffer;

internal void LinuxTerminalBufferSize(int Width, int Height) {

    // TODO: (marco) Bulletproof this
    // maybe not free first but later

    if (TerminalBuffer.Cells) {
        free(TerminalBuffer.Cells);
    }
    TerminalBuffer.Width = Width;
    TerminalBuffer.Height = Height;

    TerminalBuffer.Cells = (chtype *)malloc(sizeof(chtype) * Width * Height);
    if (TerminalBuffer.Cells) {
        for (int i = 0; i < Width * Height; ++i) {
            TerminalBuffer.Cells[i] = ' ';
        }
    }
    else {
        // TODO: Logging
    }
}
// TODO: (marco) This actually uses nCurses to write to the terminal line by line. line is more optimized then cell I believe. Check this maybe?
internal void LinuxPresentBuffer(int Width, int Height) {
    erase();

    for (int y = 0; y < Height; ++y) {
        move(y, 0);
        addchnstr(&TerminalBuffer.Cells[y * Width], Width);
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
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // TODO:(marco) Take this out and replace with a function, a clamp to ensure valid parameter
    // and a struct possible defined in header file.
    struct winsize WindowSize = {};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize) == 0) {

        resizeterm(WindowSize.ws_row, WindowSize.ws_col);
        LinuxTerminalBufferSize(WindowSize.ws_col, WindowSize.ws_row);
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        // clearok(stdscr, TRUE);
        // refresh();

        Running = 1;

        while (Running) {
            if (ResizeRequested) {
                ResizeRequested = 0;

                ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);
                if (WindowSize.ws_row > 0 && WindowSize.ws_col > 0) {
                    resizeterm(WindowSize.ws_row, WindowSize.ws_col);
                    LinuxTerminalBufferSize(WindowSize.ws_col, WindowSize.ws_row);
                }
                else {
                    // TODO: (marco) logging
                }
            }

            // TODO: (marco) create UpdateAppAndRender

            // Clear
            for (int i = 0; i < TerminalBuffer.Width * TerminalBuffer.Height; ++i) {
                TerminalBuffer.Cells[i] = ' ';
            }

            // 'Render' as white with a box arround
            for (int y = 0; y < TerminalBuffer.Height; ++y) {
                for (int x = 0; x < TerminalBuffer.Width; ++x) {
                    int Index = y * TerminalBuffer.Width + x;
                    if (y == 0 || y == TerminalBuffer.Height - 1) {
                        TerminalBuffer.Cells[Index] = '-' | COLOR_PAIR(1);
                    }
                    else if (x == 0 || x == TerminalBuffer.Width - 1) {
                        TerminalBuffer.Cells[Index] = '|' | COLOR_PAIR(1);
                    }
                    else {
                        TerminalBuffer.Cells[Index] = ' ' | COLOR_PAIR(1);
                    }
                }
            }
            LinuxPresentBuffer(TerminalBuffer.Width, TerminalBuffer.Height);
            getch();
        }
    }
    else {
        // TODO:(marco) Logging
    }
    endwin();
    return 0;
}
