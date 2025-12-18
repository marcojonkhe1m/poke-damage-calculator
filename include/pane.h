#pragma once

#include <ncurses.h>

struct Pane {
    int height;
    int width;
    int starty;
    int startx;
    WINDOW* window;
};

WINDOW* create_pane(int height, int width, int starty, int startx);
void destroy_pane(Pane* pane);
