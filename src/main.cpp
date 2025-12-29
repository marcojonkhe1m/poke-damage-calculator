#include "file_handler.h"
#include "window_manager.h"

// random comment for the streak;
// day 2 of random streak comment. Returning soon!
int main() {
    int Key;
    window_manager WindowManager {};
    const char *File = "data/helloworld.txt";

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

        void *Memory = ReadEntireFile(File);
        FreeEntireFile(Memory);
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
