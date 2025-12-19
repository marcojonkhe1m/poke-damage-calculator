#pragma once

#include <ncurses.h>

struct pane {
    int Height;
    int Width;
    int StartY;
    int StartX;
    WINDOW* Window;
};

WINDOW* PaneCreateWindow(pane* Pane);
void DestroyPane(pane* Pane);
