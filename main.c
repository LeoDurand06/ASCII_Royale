#include <ncursesw/ncurses.h>
#include <unistd.h>
#include <locale.h> // UTF-8 support

#define DELAY 50000

int main() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    if (FALSE == has_colors())
    {
        endwin();
        printf("Your terminal does not support color\n");
        return 1;
    }

    start_color();
    // color pairs (ID, foreground, background).
    init_color(COLOR_BLACK, 0,0,0);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);


    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int y = yMax ;
    int x = xMax / 2;
    int startY=0,startX=0;

    int menu_sel=0;
    
    char ch1 = getch();

    while (1)
    {
        clear();

        attron(COLOR_PAIR(1));

       // mvprintw(startY+3, x-33, "┌─────────────────────────────────────────────────────────────────┐");
        mvprintw(startY+4,     x-35, "       _    ____   ________      ____                   _        ");
        mvprintw(startY +5, x-36, "      / \\  / ___| / ___\\ \\ \\    |  _ \\ ___  _   _  __ _| | ___   ");
        mvprintw(startY + 6, x-36, "     / _ \\ \\___ \\| |    \\ \\ \\   | |_) / _ \\| | | |/ _` | |/ _ \\  ");
        mvprintw(startY + 7, x-36, "    / ___ \\ ___) | |___  \\ \\ \\  |  _ | (_) | |_| | (_| | |  __/  ");
        mvprintw(startY + 8, x-36, "   /_/   \\_\\____/ \\____|  \\_\\_\\ |_| \\_\\___/ \\__, |\\__,_|_|\\___|  ");
        mvprintw(startY + 9, x-36, "--------------------------------------------|___/----------------");

        if(menu_sel==0){
            attron(A_STANDOUT);
            mvprintw(startY+13, x-49, "| Online Multiplayer |");
            attroff(A_STANDOUT);
            mvprintw(startY+13, x-21, "| Local Multiplayer |");
            mvprintw(startY+13, x+10, "| Settings |");
            mvprintw(startY+13, x+29, "| Quit |");
        }
        if(menu_sel==1){
            
            mvprintw(startY+13, x-49, "| Online Multiplayer |");
            attron(A_STANDOUT);
            mvprintw(startY+13, x-21, "| Local Multiplayer |");
            attroff(A_STANDOUT);
            mvprintw(startY+13, x+10, "| Settings |");
            mvprintw(startY+13, x+29, "| Quit |");
        }
        if(menu_sel==2){
            
            mvprintw(startY+13, x-49, "| Online Multiplayer |");
            mvprintw(startY+13, x-21, "| Local Multiplayer |");
            attron(A_STANDOUT);
            mvprintw(startY+13, x+10, "| Settings |");
            attroff(A_STANDOUT);
            mvprintw(startY+13, x+29, "| Quit |");
        }
        if(menu_sel==3){
            mvprintw(startY+13, x-49, "| Online Multiplayer |");
            mvprintw(startY+13, x-21, "| Local Multiplayer |");
            mvprintw(startY+13, x+10, "| Settings |");
            attron(A_STANDOUT);
            mvprintw(startY+13, x+29, "| Quit |");
            attroff(A_STANDOUT);
        }

        int ch1 = getch();

        if (ch1 == KEY_LEFT) {
            if (menu_sel - 1 < 0) 
                menu_sel = 3; 
            else 
                menu_sel--;
        } 
        else if (ch1 == KEY_RIGHT) {
            if (menu_sel + 1 > 3) 
                menu_sel = 0; 
            else 
                menu_sel++;
        }


        mvprintw(yMax - 6, x - 30, "[ ◄ ] Left   [ ► ] Right   [ ↵ / Enter ] Select");

        mvprintw(yMax - 2,x-13,"By Adrice & Heroharley");

        refresh();
        usleep(DELAY);
    }

    endwin();

    return 0;
}