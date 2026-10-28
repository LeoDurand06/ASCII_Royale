#ifndef MENU_COMMON_H
#define MENU_COMMON_H

#define ARRAY_LEN(a) ((int)(sizeof(a) / sizeof((a)[0])))

/* Special return values of menu_run(); item indices are >= 0. */
#define MENU_ESC  (-1)
#define MENU_QUIT (-2)

typedef struct {
    const char *const *title; /* art/text lines, WITHOUT borders */
    int title_lines;
    const char *const *items; /* plain labels, WITHOUT borders */
    int item_count;
    const char *footer; /* one-line key hint, may be NULL */
} MenuSpec;

/*
 * Runs a menu until the user picks something.
 * Returns the item index, MENU_ESC (Esc) or MENU_QUIT (Q).
 * *selected is read as the starting highlight and updated.
 * Requires ncurses to be initialized by the caller.
 */
int menu_run(const MenuSpec *menu, int *selected);

/* Shows a centered message and waits for a key. */
void menu_message(const char *text);

#endif