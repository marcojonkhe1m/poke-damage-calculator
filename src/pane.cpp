#include "pane.h"

WINDOW* CreatePane(pane* Pane) {
    WINDOW* Window = newwin(Pane->Height, Pane->Width, Pane->StartY, Pane->StartX);
    Pane->Window = Window;

    // Voor nu krijgen alle windows een border, dit wordt later in draw geregeld en met focus.
    box(Window, Pane->StartY, Pane->StartX);
    wrefresh(Window);

    return Window;
};

void DestroyPane(pane* Pane) {
    wclear(Pane->Window);
    wrefresh(Pane->Window);
    delwin(Pane->Window);
};
