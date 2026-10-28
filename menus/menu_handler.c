#include "menu_handler.h"
#include "local_mp.h"
#include "main_menu.h"
#include "menu_common.h"

/* Returns 0 if the user asked to quit the whole application. */
static int run_local_mp(void)
{
    for (;;) {
        switch (local_mp_menu()) {
            case LMP_INVITE_PLAYERS:
                menu_message("Invite Players: not implemented yet.");
                break;
            case LMP_OPTIONS:
                menu_message("Options: not implemented yet.");
                break;
            case LMP_START_GAME:
                menu_message("Start Game: not implemented yet.");
                break;
            case LMP_BACK:
                return 1;
            case LMP_QUIT:
                return 0;
        }
    }
}

void menu_handler(void)
{
    int running = 1;

    while (running) {
        switch (main_menu()) {
            case MAIN_MENU_LOCAL_MULTIPLAYER:
                running = run_local_mp();
                break;
            case MAIN_MENU_ONLINE_MULTIPLAYER:
                menu_message("Online Multiplayer: not implemented yet.");
                break;
            case MAIN_MENU_SETTINGS:
                menu_message("Settings: not implemented yet.");
                break;
            case MAIN_MENU_QUIT:
                running = 0;
                break;
        }
    }
}