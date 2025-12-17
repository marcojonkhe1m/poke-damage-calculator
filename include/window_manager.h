#pragma once

#include <ncurses.h>

typedef struct WindowManager WindowManager;

WindowManager* create_window_manager();
void destroy_window_manager(WindowManager* wm);

void add_window(WindowManager* wm, WINDOW* handle);
void remove_window(WindowManager* wm, WINDOW* handle);

void set_focused_window(WindowManager* wm, WINDOW* handle);
void handle_input(WindowManager* wm, int key);
void update_all(WindowManager* wm);
void draw_all(WindowManager* wm);
