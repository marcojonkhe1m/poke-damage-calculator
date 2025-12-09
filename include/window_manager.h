#pragma once

#include "window.h"

typedef struct {
  WINDOW **layer_stack;
  WINDOW *current_window;
}
