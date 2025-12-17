#include "window_manager.h"
#include <ncurses.h>

struct WindowManager {
    WINDOW* windows[5];
    int window_count;
};

// Gekozen voor een heap allocatie, terwijl er meestal maar 1 wm is, omdat dit de levenscyclus
// van de wm expliciet maakt en dat ik er zelf controle over heb.
WindowManager* create_window_manager() {
    WindowManager* wm = new WindowManager;
    return wm;
}

void destroy_window_manager(WindowManager* wm) {
    delete wm;
    return;
}

void add_window(WindowManager* wm, WINDOW* handle) {
    if (wm->window_count == (sizeof(windows) / sizeof(WINDOW*))) {
        return;
    }

    // TODO (marco): z-index implementatie
    wm->window_count += 1;
    wm->windows[wm->window_count] = handle;
    return;
}

void remove_window(WindowManager* wm, WINDOW* handle) {
    for (int i = 0; i < wm->window_count; i++) {
        if (wm->windows[i] == handle) {

            WINDOW* new_arr[5];
            int k = 0;
            for (int j = 0; j < wm->window_count; j++) {
                if (j == i) {
                    k++;
                }
                new_arr[j] = wm->windows[k];
                k++;
            }
        }
    }
}

void set_focused_window(WindowManager* wm, WINDOW* handle);
void handle_input(WindowManager* wm, int key);
void update_all(WindowManager* wm);
void draw_all(WindowManager* wm);
