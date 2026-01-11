#include <iostream>
#include <ncurses.h>
#include <signal.h>
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
// TODO: (marco) This actually uses nCurse to write to the terminal line by line. line is more optimized then cell I believe. Check this maybe?
internal void LinuxPresentBuffer(int X, int Y, int Width, int Height) {
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

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    Running = 1;

    while (Running) {
        if (ResizeRequested) {
            ResizeRequested = 0;

            struct winsize WindowSize;
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

        // 'Render' as white
        for (int i = 0; i < TerminalBuffer.Width * TerminalBuffer.Height; ++i) {
            TerminalBuffer.Cells[i] = ' ' | COLOR_PAIR(1);
        }

        getch();
    }
    endwin();
    return 0;
}
