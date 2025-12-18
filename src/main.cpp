#include "window_manager.h"
#include <ncurses.h>
#include <string.h>

// TODO (marco): wil ik de ncurses window * in de struct zetten?
struct Window {
    int height;
    int width;
    int starty;
    int startx;
    const char* title;
};

WINDOW* create_bordered_window(struct Window* win_struct) {
    WINDOW* win = newwin(win_struct->height, win_struct->width, win_struct->starty, win_struct->startx);
    box(win, 0, 0);
    keypad(win, TRUE);

    if (win_struct->title != NULL) {
        mvwprintw(win, 0, 2, "%s", win_struct->title);
    }
    wrefresh(win);

    return win;
}

void destroy_window(WINDOW* win) {
    delwin(win);
}

int main() {
    int key;
    Window win = {};

    // wm initialiser
    WindowManager* window_manager = create_window_manager();
    if (!window_manager) {
        return -1;
    }

    win.height = LINES;
    win.width = COLS;
    win.starty = 0;
    win.startx = 0;
    win.title = " Pokemon Damage Calculator v0.1 ";

    initscr();
    cbreak();
    noecho();

    keypad(stdscr, TRUE);

    refresh();

    WINDOW* main_win = create_bordered_window(&win);

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
