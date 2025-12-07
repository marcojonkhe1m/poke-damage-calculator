#include <ncurses.h>

typedef struct {
  int height;
  int width;
  int xstart;
  int ystart;
  WINDOW *window;
} win;

int main() {
  win header;

  initscr();
  cbreak();
  noecho();

  header.height = 6;
  header.width = COLS;
  header.xstart = 0;
  header.ystart = 0;
  refresh();

  header.window =
      newwin(header.height, header.width, header.ystart, header.xstart);
  box(header.window, 0, 0);
  mvwprintw(header.window, 0, 3, "Pokemon Damage Calculator v0.1");
  mvwprintw(header.window, header.height / 2, header.width / 2, "Storm Silver");
  wrefresh(header.window);

  getch();
  delwin(header.window);
  endwin();
  return 0;
}
