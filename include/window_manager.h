#pragma once

#include "pane.h"

struct window_manager {
    pane* Panes[5];
    int PaneCount;
};

void WmAddPane(window_manager* Wm, pane* Pane);
void WmRemovePane(window_manager* Wm, pane* Pane);

/*
void WmMakePaneFocused(window_manager* Wm, pane* Pane);
void WmHandleInput(window_manager* Wm, int Key);
void WmUpdateAndRender(window_manager* Wm);
*/
