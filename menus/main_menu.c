#include "main_menu.h"
#include "menu_common.h"

static const char *const title[] = {
    " __  __       _         __  __",
    "|  \\/  | __ _(_)_ __   |  \\/  | ___ _ __  _   _",
    "| |\\/| |/ _` | | '_ \\  | |\\/| |/ _ \\ '_ \\| | | |",
    "| |  | | (_| | | | | | | |  | |  __/ | | | |_| |",
    "|_|  |_|\\__,_|_|_| |_| |_|  |_|\\___|_| |_|\\__,_|"
};

static const char *const items[] = {
    "Local Multiplayer",
    "Online Multiplayer",
    "Settings",
    "Quit"
};

static const MainMenuResult results[] = {
    MAIN_MENU_LOCAL_MULTIPLAYER,
    MAIN_MENU_ONLINE_MULTIPLAYER,
    MAIN_MENU_SETTINGS,
    MAIN_MENU_QUIT
};

MainMenuResult main_menu(void)
{
    static int selected = 0;   /* remember highlight when coming back */

    const MenuSpec spec = {
        title, ARRAY_LEN(title),
        items, ARRAY_LEN(items),
        "[Up/Down] Navigate  [Enter/Click] Select  [Q] Quit"
    };

    for (;;) {
        int r = menu_run(&spec, &selected);

        if (r == MENU_QUIT)
            return MAIN_MENU_QUIT;
        if (r >= 0)
            return results[r];
        /* MENU_ESC: nothing to go back to, keep showing the menu */
    }
}