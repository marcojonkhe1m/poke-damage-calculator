#include <ncurses.h>

// TODO (marco): wil ik de ncurses window * in de struct zetten?
struct Window {
    int height;
    int width;
    int starty;
    int startx;
};

int main() {
    Window win = {};

    win.height = LINES;
    win.width = COLS;
    win.starty = 0;
    win.startx = 0;

    initscr();
    cbreak();
    noecho();

    refresh();

    WINDOW* localWindow = newwin(win.height, win.width, win.starty, win.startx);
    box(localWindow, 0, 0);
    wrefresh(localWindow);

    getch();

    delwin(localWindow);

    return 0;
}
