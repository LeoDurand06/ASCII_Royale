
#ifndef LOCAL_MP_H
#define LOCAL_MP_H

typedef enum {
    LMP_BACK,
    LMP_QUIT,
    LMP_INVITE_PLAYERS,
    LMP_OPTIONS,
    LMP_START_GAME
} LocalMPResult;

/*
 * Requires ncurses to have been initialized by the caller.
 * Does not call initscr(), endwin(), main_menu(), or exit().
 */
LocalMPResult local_mp_menu(void);

#endif
