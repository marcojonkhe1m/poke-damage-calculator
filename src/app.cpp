#include "app.h"
#include <curses.h>

void run_app() {
  init();

  bool running = true;

  int x;
  int y;

  getmaxyx(stdscr, y, x);

  y = y * 0.5;
  x = (x * 0.5) - 6;

  mvwprintw(stdscr, y, x, "Hello World!");
  mvwprintw(stdscr, 2, 2, "Hello World!");
  refresh();

  // while (running) {
  //  handle_events
  //
  //  update
  //
  //  draw
  //}

  //  close_app();

  getch();
  endwin();
}

void init() {
  initscr();
  noecho();
  cbreak();
}
