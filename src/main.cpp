#include "window_manager.h"
#include <string.h>

int main() {
    int key;
    window_manager WindowManager {};

    // init ncurses
    initscr();
    cbreak();
    noecho();

    keypad(stdscr, TRUE);

    refresh();

    // init panes
    pane MainPane {};
    createPane(&MainPane

    while (1) {
        key = wgetch(main_win);
        const char* text;

        switch (key) {
        case KEY_UP:
            text = "arrow up pressed";
            mvwprintw(main_win, LINES / 2, (COLS - strlen(text)) / 2, "%s", text);
            wrefresh(main_win);
            break;
        case KEY_DOWN:
            text = "arrow down pressed";
            mvwprintw(main_win, LINES / 2, (COLS - strlen(text)) / 2, "%s", text);
            wrefresh(main_win);
            break;
        case KEY_LEFT:
            text = "arrow left pressed";
            mvwprintw(main_win, LINES / 2, (COLS - strlen(text)) / 2, "%s", text);
            wrefresh(main_win);
            break;
        case KEY_RIGHT:
            text = "arrow right pressed";
            mvwprintw(main_win, LINES / 2, (COLS - strlen(text)) / 2, "%s", text);
            wrefresh(main_win);
            break;
        default:
            text = "press an arrow key";
            mvwprintw(main_win, LINES / 2, (COLS - strlen(text)) / 2, "%s", text);
            wrefresh(main_win);
            break;
        }
        break;
    }

    getch();

    destroy_window(main_win);
    refresh();
    endwin();

    return 0;
}
