#pragma once

#include <ncurses.h>

typedef struct {
  int height;
  int width;
  int starty;
  int startx;
  WINDOW *window;
} win;

void update();

void draw();
