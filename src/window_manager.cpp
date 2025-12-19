#include "window_manager.h"
#include "pane.h"

void WmAddPane(window_manager* Wm, pane* Pane) {
    if (Wm->PaneCount == (sizeof(Wm->Panes) / sizeof(Wm->Panes[0]))) {
        return;
    }

    // TODO (marco): z-index implementatie
    Wm->PaneCount += 1;
    Wm->Panes[Wm->PaneCount] = Pane;
    return;
}

void WmRemovePane(window_manager* Wm, pane* Pane) {
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
    DestroyPane(Pane);
}

/*
void set_focused_window(WindowManager* wm, WINDOW* handle);
void handle_input(WindowManager* wm, int key);
void update_all(WindowManager* wm);
void draw_all(WindowManager* wm);
*/
