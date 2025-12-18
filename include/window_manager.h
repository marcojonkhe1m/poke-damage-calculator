#pragma once

#include "pane.h"

struct WindowManager;

WindowManager* create_window_manager();
void destroy_window_manager(WindowManager* wm);

void add_pane(WindowManager* wm, Pane* pane);
void remove_pane(WindowManager* wm, Pane* pane);

void set_focused_pane(WindowManager* wm, Pane* pane);
void handle_input(WindowManager* wm, int key);
void update_all(WindowManager* wm);
void draw_all(WindowManager* wm);
