#pragma once

#include "pane.h"

struct window_manager {
    pane *Panes[5];
    int PaneCount;
};

void WmDestroy(window_manager *Wm);

void WmAddPane(window_manager *Wm, pane *Pane);
void WmRemovePane(window_manager *Wm, pane *Pane);

void WmHandleInput(window_manager *Wm, int Key);

/*
void WmMakePaneFocused(window_manager* Wm, pane* Pane);
void WmUpdateAndRender(window_manager* Wm);
*/
