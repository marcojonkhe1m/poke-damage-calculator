#pragma once

#include <ncurses.h>

struct pane {
    int Height;
    int Width;
    int StartY;
    int StartX;
    WINDOW *Window;
};

pane *PaneInit(pane *Pane);
void PaneDestroy(pane *Pane);

void PaneHandleInput(pane *Pane, int Key);
