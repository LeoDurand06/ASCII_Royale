#include "menus/menu_handler.h"
#include <locale.h>
#include <ncursesw/ncurses.h>

int main(void)
{
    setlocale(LC_ALL, "");

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
    }

    menu_handler();

    endwin();
    return 0;
}