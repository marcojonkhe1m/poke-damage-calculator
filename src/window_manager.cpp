#include "window_manager.h"
#include "pane.h"

struct WindowManager {
    Pane* panes[5];
    int pane_count;
};

// Gekozen voor een heap allocatie, terwijl er meestal maar 1 wm is, omdat dit de levenscyclus
// van de wm expliciet maakt en dat ik er zelf controle over heb.
WindowManager* create_window_manager() {
    WindowManager* wm = new WindowManager;
    return wm;
}

void destroy_window_manager(WindowManager* wm) {
    for (int i = 0; i < wm->pane_count; i++) {
        destroy_pane(wm->panes[i]);
    }
    delete wm;
    return;
}

void add_pane(WindowManager* wm, Pane* pane) {
    if (wm->pane_count == (sizeof(wm->panes) / sizeof(wm->panes[0]))) {
        return;
    }

    // TODO (marco): z-index implementatie
    wm->pane_count += 1;
    wm->panes[wm->pane_count] = pane;
    return;
}

void remove_pane(WindowManager* wm, Pane* pane) {
    int index = -1;
    for (int i = 0; i < wm->pane_count; i++) {
        if (wm->panes[i] == pane) {
            index = i;
        }

        if (index == -1) {
            return;
        }

        for (int i = index; i < wm->pane_count - 1; i++) {
            wm->panes[i] = wm->panes[i + 1];
        }
        wm->pane_count--;
    }
    destroy_pane(pane);
}

/*
void set_focused_window(WindowManager* wm, WINDOW* handle);
void handle_input(WindowManager* wm, int key);
void update_all(WindowManager* wm);
void draw_all(WindowManager* wm);
*/
