#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <ncursesw/ncurses.h>

#define DELAY 100
#define MENU_COUNT 4

const char *menu[] = {
    "| Online Multiplayer |",
    "| Local Multiplayer |",
    "| Settings |",
    "| Quit |"
};

int main(void)
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    if (!has_colors()) {
        endwin();
        printf("Your terminal does not support colors.\n");
        return 1;
    }

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int x = xMax / 2;
    int menu_sel = 0;
    int running = 1;

    while (running) {
        erase();
        attron(COLOR_PAIR(1));

        mvprintw(4, x-35, "       _    ____   ________      ____                   _        ");
        mvprintw(5, x-36, "      / \\  / ___| / ___\\ \\ \\    |  _ \\ ___  _   _  __ _| | ___   ");
        mvprintw(6, x-36, "     / _ \\ \\___ \\| |    \\ \\ \\   | |_) / _ \\| | | |/ _` | |/ _ \\  ");
        mvprintw(7, x-36, "    / ___ \\ ___) | |___  \\ \\ \\  |  _ | (_) | |_| | (_| | |  __/  ");
        mvprintw(8, x-36, "   /_/   \\_\\____/ \\____|  \\_\\_\\ |_| \\_\\___/ \\__, |\\__,_|_|\\___|  ");
        mvprintw(9, x-36, "--------------------------------------------|___/----------------");

        int positions[] = {x-49, x-21, x+10, x+29};

        for (int i = 0; i < MENU_COUNT; i++) {
            
            if (i == menu_sel) attron(A_STANDOUT);

            mvprintw(13, positions[i], "%s", menu[i]);

            if (i == menu_sel) attroff(A_STANDOUT);

        }

        mvprintw(yMax-6, 2,
                 "[ Left/Right ] Navigate  [ Enter ] Select  [ Click ] Choose");
        mvprintw(yMax-2, x-13, "By Adrice & Heroharley");

        /* refresh only after drawing the complete screen */
        refresh();

        int ch = getch();

        if (ch == KEY_LEFT) {
            if (menu_sel - 1 < 0) menu_sel = 3; 
            else menu_sel--;
        }
        else if (ch == KEY_RIGHT) {
            if (menu_sel + 1 > 3) menu_sel = 0; 
            else menu_sel++;
        }
        else if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) {

            /* if we do ENTER when [QUIT] button is selected */
            if (menu_sel == 3) running = 0;
        }

        /* when we click with mouse */
        else if (ch == KEY_MOUSE) {
            MEVENT event;

            if (getmouse(&event) == OK && event.y == 13) {

                for (int i = 0; i < MENU_COUNT; i++) {
                    int width = (int)strlen(menu[i]);

                    if (event.x >= positions[i] &&
                        event.x < positions[i] + width) {
                        menu_sel = i;

                        /* if we select [QUIT] button */
                        if (i == 3)
                            running = 0;

                        break;
                    }

                }

            }

        }

        usleep(DELAY);
    }

    endwin();
    return 0;
}