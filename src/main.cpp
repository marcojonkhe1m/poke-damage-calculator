#include "window_manager.h"

int main() {
    int Key;
    window_manager WindowManager {};

    // init ncurses
    initscr();
    cbreak();
    noecho();

    keypad(stdscr, TRUE);

    refresh();

    // init panes
    pane MainPane {};
    MainPane.Height = LINES;
    MainPane.Width = COLS;

    if (!PaneInit(&MainPane)) {
        return -1;
    }

    WmAddPane(&WindowManager, &MainPane);

    bool Running = true;

    while (Running) {
        Key = getch();
        const char *text;

        if (Key != KEY_F(1)) {
            WmHandleInput(&WindowManager, Key);
        }

        else {
            Running = false;
        }
    }

    WmDestroy(&WindowManager);
    refresh();
    endwin();

    return 0;
}
