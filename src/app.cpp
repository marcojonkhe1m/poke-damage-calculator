#include "app.h"
#include <curses.h>

// voor de streak;
//
void run_app() {
  WINDOW *win;
  int startx, starty, width, height;

  init();

  bool running = true;

  int x;
  int y;

  getmaxyx(stdscr, y, x);

  height = 6;
  width = COLS - 1;

  win = newwin(height, width, 1, 1);
  refresh();

  box(win, 0, 0);
  mvwprintw(win, 0, 1, "Pokemon Damage Calculator v 0.1");
  wrefresh(win);

  // while (running) {
  //  handle_events
  //
  //  update
  //
  //  draw
  //}

  //  close_app();

  getch();
  delwin(win);
  endwin();
}

void init() {
  initscr();
  noecho();
  cbreak();
}
