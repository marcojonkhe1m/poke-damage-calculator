#include <iostream>
#include <ncurses.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define internal static
#define local_persist static
#define global_variable static

// TODO: (marco): This is a global for now
global_variable volatile sig_atomic_t Running;
global_variable volatile sig_atomic_t ResizeRequested;

internal void ResizeWindow(int Width, int Height) {
}

internal void SignalHandler(int Signo, siginfo_t *Info, void *Context) {
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

struct screen {
    int Height;
    int Width;
    int StartX;
    int StartY;
    WINDOW *screen;
};

int main() {

    struct sigaction Sa = {};
    Sa.sa_sigaction = SignalHandler;
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
    Running = 1;

    while (Running) {
        if (ResizeRequested) {
            ResizeRequested = 0;
            std::cerr << "Debug: SIGWINCH";

            struct winsize WindowSize;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &WindowSize);
            if (WindowSize.ws_row > 0 && WindowSize.ws_col > 0) {
                ResizeWindow(WindowSize.ws_col, WindowSize.ws_row);
            }
            else {
                // TODO: (marco) logging
            }
        };

        screen MainScreen = {};

        MainScreen.Height = LINES;
        MainScreen.Width = COLS;
        MainScreen.StartX = 0;
        MainScreen.StartY = 0;

        init_pair(1, COLOR_WHITE, COLOR_BLACK);

        MainScreen.screen = newwin(MainScreen.Height, MainScreen.Width, MainScreen.StartY, MainScreen.StartX);
        if (MainScreen.screen != NULL) {
            box(MainScreen.screen, 0, 0);
            wrefresh(MainScreen.screen);

            wattron(MainScreen.screen, COLOR_PAIR(1));
            mvwprintw(MainScreen.screen, MainScreen.Height / 2, MainScreen.Width / 2, "HelloWorld!");
            wrefresh(MainScreen.screen);

            getch();
            delwin(MainScreen.screen);
        }

        else {
            // TODO: Logging
        }
    }
    endwin();
    return 0;
}
