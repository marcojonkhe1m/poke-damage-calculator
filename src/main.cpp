#include <iostream>
#include <ncurses.h>

int main() {
  std::cout << "\n===================================" << std::endl;
  std::cout << "Welcome to Poke Damage Calculator!" << std::endl;
  std::cout << "===================================" << std::endl;
  std::cout << "\nChoose an option: " << std::endl;
  std::cout << "\n1. Select Pokemon" << std::endl;
  std::cout << "2. Exit" << std::endl;

  int input;
  std::cout << "\nEnter option: ";
  std::cin >> input;

  switch (input) {
  case 1:
    char *name;

    std::cout << "\n==============================" << std::endl;
    std::cout << "Pokemon selection screen" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "\nPlease enter Pokemon name: ";
    std::cin >> name;
    std::cout << "\n --> " << name << " selected by Player!\n" << std::endl;

    std::cout << "\nPlease enter Opponent's Pokemon name: ";
    std::cin >> name;
    std::cout << "\n --> " << name << " selected by Opponent!\n" << std::endl;

    break;

  case 2:
    std::cout << "\nExiting.......\n" << std::endl;
    break;

  default:
    std::cout << "\nInput not valid...\nExiting.....\n" << std::endl;
  }

  return 0;
}
