#pragma once

#include "pane.h"

struct window_manager {
    pane *Panes[5];
    int PaneCount;
};

struct key_state {
    bool IsDown;
    int HalfTransitionCount;
};

struct keyboard_input {
    key_state MoveUp;
    key_state MoveDown;
    key_state MoveLeft;
    key_state MoveRight;

    key_state SelectCurrent;
    key_state UndoLast;

    key_state PaneMoveUp;
    key_state PaneMoveDown;
    key_state PaneMoveLeft;
    key_state PaneMoveRight;
};

void WmDestroy(window_manager *Wm);

void WmAddPane(window_manager *Wm, pane *Pane);
void WmRemovePane(window_manager *Wm, pane *Pane);

void WmHandleInput(window_manager *Wm, int Key);

/*
void WmMakePaneFocused(window_manager* Wm, pane* Pane);
void WmUpdateAndRender(window_manager* Wm);
*/
