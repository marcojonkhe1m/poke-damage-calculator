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
  win my_pokemon_window;
  win enemy_pokemon_window;

  initscr();
  cbreak();
  noecho();

  header.height = 6;
  header.width = COLS;
  header.xstart = 0;
  header.ystart = 0;

  my_pokemon_window.height = LINES - header.height;
  my_pokemon_window.width = COLS / 2;
  my_pokemon_window.xstart = 0;
  my_pokemon_window.ystart = header.height;

  enemy_pokemon_window.height = LINES - header.height;
  enemy_pokemon_window.width = COLS / 2;
  enemy_pokemon_window.xstart = COLS / 2;
  enemy_pokemon_window.ystart = header.height;

  refresh();

  header.window =
      newwin(header.height, header.width, header.ystart, header.xstart);
  box(header.window, 0, 0);
  mvwprintw(header.window, 0, 3, "Pokemon Damage Calculator v0.1");
  mvwprintw(header.window, header.height / 2, header.width / 2, "Storm Silver");
  wrefresh(header.window);

  my_pokemon_window.window =
      newwin(my_pokemon_window.height, my_pokemon_window.width,
             my_pokemon_window.ystart, my_pokemon_window.xstart);
  box(my_pokemon_window.window, 0, 0);
  mvwprintw(my_pokemon_window.window, 0, 3, "Player Pokemon");
  mvwprintw(my_pokemon_window.window, my_pokemon_window.height / 2,
            my_pokemon_window.width / 2, "Bulbasaur lv. 100");
  wrefresh(my_pokemon_window.window);

  enemy_pokemon_window.window =
      newwin(enemy_pokemon_window.height, enemy_pokemon_window.width,
             enemy_pokemon_window.ystart, enemy_pokemon_window.xstart);
  box(enemy_pokemon_window.window, 0, 0);
  mvwprintw(enemy_pokemon_window.window, 0, 3, "Enemy Pokemon");
  mvwprintw(enemy_pokemon_window.window, my_pokemon_window.height / 2,
            enemy_pokemon_window.width / 2, "Charmander lv. 100");
  wrefresh(enemy_pokemon_window.window);

  getch();
  delwin(header.window);
  delwin(my_pokemon_window.window);
  delwin(my_pokemon_window.window);
  endwin();
  return 0;
}
