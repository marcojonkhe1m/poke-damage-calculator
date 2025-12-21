#include "pane.h"
#include <string.h>

pane *PaneInit(pane *Pane) {
    WINDOW *Window = newwin(Pane->Height, Pane->Width, Pane->StartY, Pane->StartX);
    if (!Window) {
        return NULL;
    }
    Pane->Window = Window;

    // Voor nu krijgen alle windows een border, dit wordt later in draw geregeld en met focus.
    box(Window, Pane->StartY, Pane->StartX);
    wrefresh(Window);

    return Pane;
};

void PaneDestroy(pane *Pane) {
    wclear(Pane->Window);
    wrefresh(Pane->Window);
    delwin(Pane->Window);
};

void PaneHandleInput(pane *Pane, int Key) {
    const char *text;
    switch (Key) {
    case KEY_UP:
        text = "arrow up pressed";
        mvwprintw(Pane->Window, LINES / 2, (COLS - strlen(text)) / 2, "%s", text);
        wrefresh(Pane->Window);
        break;
    case KEY_DOWN:
        text = "arrow down pressed";
        mvwprintw(Pane->Window, LINES / 2, (COLS - strlen(text)) / 2, "%s", text);
        wrefresh(Pane->Window);
        break;
    case KEY_LEFT:
        text = "arrow left pressed";
        mvwprintw(Pane->Window, LINES / 2, (COLS - strlen(text)) / 2, "%s", text);
        wrefresh(Pane->Window);
        break;
    case KEY_RIGHT:
        text = "arrow right pressed";
        mvwprintw(Pane->Window, LINES / 2, (COLS - strlen(text)) / 2, "%s", text);
        wrefresh(Pane->Window);
        break;
    default:
        text = "press an arrow key";
        mvwprintw(Pane->Window, LINES / 2, (COLS - strlen(text)) / 2, "%s", text);
        wrefresh(Pane->Window);
        break;
    }
}
