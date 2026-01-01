#include "file_handler.h"
#include "window_manager.h"
#include <iostream>

// random comment for the streak;
// day 2 of random streak comment. Returning soon!
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

        const char *Filename = PROJECT_ROOT "/src/main.cpp";
        read_file_result File = ReadEntireFile(Filename);
        std::cout << "contentsSize: " << File.Contents << std::endl;
        if (File.Contents) {
            WriteEntireFile(PROJECT_ROOT "/data/helloworld.txt", File.Contents, File.ContentsSize);
            FreeEntireFile(File.Contents);
        }

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
