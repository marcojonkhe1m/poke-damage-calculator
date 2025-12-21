#include "window_manager.h"
#include "pane.h"

void WmDestroy(window_manager *Wm) {
    if (Wm == NULL) {
        return;
    }

    while (Wm->PaneCount > 0) {
        WmRemovePane(Wm, Wm->Panes[0]);
    }
}
void WmAddPane(window_manager *Wm, pane *Pane) {
    if (Wm->PaneCount == (sizeof(Wm->Panes) / sizeof(Wm->Panes[0]))) {
        return;
    }

    // TODO (marco): z-index implementatie
    Wm->Panes[Wm->PaneCount] = Pane;
    Wm->PaneCount += 1;
    return;
}

void WmRemovePane(window_manager *Wm, pane *Pane) {
    int Index = -1;
    for (int i = 0; i < Wm->PaneCount; i++) {
        if (Wm->Panes[i] == Pane) {
            Index = i;
        }

        if (Index == -1) {
            return;
        }

        for (int i = Index; i < Wm->PaneCount - 1; i++) {
            Wm->Panes[i] = Wm->Panes[i + 1];
        }
        Wm->PaneCount--;
    }
    PaneDestroy(Pane);
}

void WmHandleInput(window_manager *Wm, int Key) {
    // TODO (marco): Focused pane switching toe voegen
    pane *FocusedPane = Wm->Panes[Wm->PaneCount - 1];
    PaneHandleInput(FocusedPane, Key);
}
/*
void set_focused_window(WindowManager* wm, WINDOW* handle);
void update_all(WindowManager* wm);
void draw_all(WindowManager* wm);
*/
