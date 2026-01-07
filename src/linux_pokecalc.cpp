#include <ncurses.h>

struct screen {
    int Height;
    int Width;
    int StartX;
    int StartY;
    WINDOW *screen;
};

int main() {
    screen MainScreen = {};

    MainScreen.Height = LINES;
    MainScreen.Width = COLS;
    MainScreen.StartX = 0;
    MainScreen.StartY = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    MainScreen.screen = newwin(MainScreen.Height, MainScreen.Width, MainScreen.StartY, MainScreen.StartX);
    if (MainScreen.screen) {
        box(MainScreen.screen, 0, 0);

        getch();

        delwin(MainScreen.screen);
    }
    else {
        // TODO: Logging
    }
    endwin();
    return 0;
}
