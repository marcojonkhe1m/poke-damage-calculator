#include "pane.h"

pane* PaneInit(pane* Pane) {
    WINDOW* Window = newwin(Pane->Height, Pane->Width, Pane->StartY, Pane->StartX);
    if (!Window) {
        return NULL;
    }
    Pane->Window = Window;

    // Voor nu krijgen alle windows een border, dit wordt later in draw geregeld en met focus.
    box(Window, Pane->StartY, Pane->StartX);
    wrefresh(Window);

    return Pane;
};

void DestroyPane(pane* Pane) {
    wclear(Pane->Window);
    wrefresh(Pane->Window);
    delwin(Pane->Window);
};
