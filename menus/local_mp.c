#include "local_mp.h"
#include "menu_common.h"

static const char *const title[] = {
    " _                    _   __  __       _ _   _       _                       ",
    "| |    ___   ___ __ _| | |  \\/  |_   _| | |_(_)_ __ | | __ _ _   _  ___ _ __ ",
    "| |   / _ \\ / __/ _` | | | |\\/| | | | | | __| | '_ \\| |/ _` | | | |/ _ \\ '__|",
    "| |__| (_) | (_| (_| | | | |  | | |_| | | |_| | |_) | | (_| | |_| |  __/ |   ",
    "|_____\\___/ \\___\\__,_|_| |_|  |_|\\__,_|_|\\__|_| .__/|_|\\__,_|\\__, |\\___|_|   ",
    "                                              |_|            |___/           "
};

static const char *const items[] = {
    "Invite Players",
    "Options",
    "Start Game",
    "Back"
};

static const LocalMPResult results[] = {
    LMP_INVITE_PLAYERS,
    LMP_OPTIONS,
    LMP_START_GAME,
    LMP_BACK
};

LocalMPResult local_mp_menu(void)
{
    static int selected = 0;

    const MenuSpec spec = {
        title, ARRAY_LEN(title),
        items, ARRAY_LEN(items),
        "[Up/Down] Navigate  [Enter/Click] Select  [Esc] Back  [Q] Quit"
    };

    int r = menu_run(&spec, &selected);

    if (r == MENU_QUIT)
        return LMP_QUIT;
    if (r >= 0)
        return results[r];
    return LMP_BACK;           /* MENU_ESC */
}