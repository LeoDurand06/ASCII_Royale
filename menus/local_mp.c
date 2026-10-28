#include <unistd.h>
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <ncursesw/ncurses.h>

const char *LMP_menu[] = {
    "| Invite Players |",
    "| Options |",
    "| Start Game |",
    "| Leave |"
    "| Back |"
};

int local_mp_menu(void)
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    start_color();
    init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int tittleY = (yMax>=100) ? (yMax/2 + 4) : ( (yMax >= 50) ? yMax / 4 - 3 : yMax / 2 - 6 );
    int menuY = (yMax >= 100 ) ? (yMax/2 + 20) : ( (xMax >= 50) ? yMax / 4 + 7 : yMax / 2 + 3 );
    int x = xMax / 2;
    int menu_sel = 0;
    int running = 2;

    while (running) {
        erase();
        attron(COLOR_PAIR(1));

        mvprintw(tittleY, x-41, "  ___        _ _              __  __       _ _   _       _                       ");
        mvprintw(tittleY + 1, x-41, " / _ \\ _ __ | (_)_ __   ___  |  \\/  |_   _| | |_(_)_ __ | | __ _ _   _  ___ _ __ ");
        mvprintw(tittleY + 2, x-41, "| | | | '_ \\| | | '_ \\ / _ \\ | |\\/| | | | | | __| | '_ \\| |/ _` | | | |/ _ \\ '__|");
        mvprintw(tittleY + 3, x-41, "| |_| | | | | | | | | |  __/ | |  | | |_| | | |_| | |_) | | (_| | |_| |  __/ |   ");
        mvprintw(tittleY + 4, x-41, " \\___/|_| |_|_|_|_| |_|\\___| |_|  |_|\\__,_|_|\\__|_| .__/|_|\\__,_|\\__, |\\___|_|   ");
        mvprintw(tittleY + 5, x-41, "--------------------------------------------------|_|------------|___/-----------");

        int positions[] = {x-49, x-21, x, x+14, x+22};

        for (int i = 0; i < 4; i++) {
            
            if (i == menu_sel) attron(A_STANDOUT);

            mvprintw(menuY, positions[i], "%s", LMP_menu[i]);

            if (i == menu_sel) attroff(A_STANDOUT);

        }

        mvprintw(menuY +5, 2,
                 "[ Left/Right ] Navigate  [ Enter ] Select  [ Click ] Choose");
        mvprintw(menuY + 7, 2, "By Adrice & Heroharley");

        /* refresh only after drawing the complete screen */
        refresh();

        int ch = getch();

        if (ch == KEY_LEFT) {
            if (menu_sel - 1 < 0) menu_sel = 3; 
            else menu_sel--;
        }
        else if (ch == KEY_RIGHT) {
            if (menu_sel + 1 > 4) menu_sel = 0; 
            else menu_sel++;
        }
        else if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) {
            
        }

        /* when we click with mouse */
        else if (ch == KEY_MOUSE) {
            MEVENT event;

            if (getmouse(&event) == OK && event.y == 13) {

                for (int i = 0; i < 5; i++) {
                    int width = (int)strlen(LMP_menu[i]);

                    if (event.x >= positions[i] &&
                        event.x < positions[i] + width) {
                        menu_sel = i;
                    
                    if (i== 4) {
                            running = 1;
                            menu_sel = 0;
                        }
                        break;
                    }

                }

            }

        }

        usleep(100);
    }

    endwin();
    return 0;
}