#ifndef MAIN_MENU_H
#define MAIN_MENU_H

typedef enum {
    MAIN_MENU_LOCAL_MULTIPLAYER,
    MAIN_MENU_ONLINE_MULTIPLAYER,
    MAIN_MENU_SETTINGS,
    MAIN_MENU_QUIT
} MainMenuResult;

/*
 * Requires ncurses to have been initialized by the caller.
 * Does not call initscr(), endwin(), or exit().
 */
MainMenuResult main_menu(void);

#endif