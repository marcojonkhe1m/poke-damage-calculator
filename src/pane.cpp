#include "pane.h"

WINDOW* create_pane(int height, int width, int starty, int startx) {
    WINDOW* pane = newwin(height, width, starty, startx);

    // Voor nu krijgen alle windows een border, dit wordt later in draw geregeld en met focus.
    box(pane, starty, startx);
    wrefresh(pane);

    return pane;
};

void destroy_pane(Pane* pane) {
    wclear(pane->window);
    wrefresh(pane->window);
    delwin(pane->window);
};
