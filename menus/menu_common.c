#include "menu_common.h"

#include <string.h>
#include <ncursesw/ncurses.h>

#define ESC_KEY            27
#define ESC_DELAY_MS       25
#define BORDER_WIDTH       4   /* "| " on the left and " |" on the right */
#define TITLE_BORDER_ROWS  2
#define GAP_ROWS           1
#define FOOTER_ROWS        1
#define ROWS_PER_ITEM      2   /* the item and the blank row below it */

typedef struct {
    int fits;
    int rows_needed, cols_needed;
    int title_x, title_y, title_width;
    int items_x, items_y, items_width;
    int label_width;
    int footer_x, footer_y;
} Layout;


static int max_int(int a, int b)
{
    return a > b ? a : b;
}

static int longest_string(const char *const *strings, int count)
{
    int longest = 0;
    for (int i = 0; i < count; ++i)
        longest = max_int(longest, (int)strlen(strings[i]));
    return longest;
}

static int center(int total, int size)
{
    return (total - size) / 2;
}

static Layout make_layout(const MenuSpec *menu)
{
    Layout layout;
    int screen_rows, screen_cols;
    getmaxyx(stdscr, screen_rows, screen_cols);

    int footer_width = menu->footer ? (int)strlen(menu->footer) : 0;
    layout.label_width = longest_string(menu->items, menu->item_count);
    layout.items_width = layout.label_width + BORDER_WIDTH;
    layout.title_width = longest_string(menu->title, menu->title_lines) + BORDER_WIDTH;

    int title_height = menu->title_lines + TITLE_BORDER_ROWS;
    int items_height = menu->item_count * ROWS_PER_ITEM - 1;

    layout.rows_needed = title_height + GAP_ROWS + items_height + GAP_ROWS + FOOTER_ROWS;
    layout.cols_needed = max_int(layout.title_width,
                                 max_int(layout.items_width, footer_width));
    layout.fits = screen_rows >= layout.rows_needed
               && screen_cols >= layout.cols_needed;

    layout.title_y  = center(screen_rows, layout.rows_needed);
    layout.items_y  = layout.title_y + title_height + GAP_ROWS;
    layout.footer_y = layout.items_y + items_height + GAP_ROWS;

    layout.title_x  = center(screen_cols, layout.title_width);
    layout.items_x  = center(screen_cols, layout.items_width);
    layout.footer_x = center(screen_cols, footer_width);

    return layout;
}


static void draw_horizontal_border(int y, int x, int width)
{
    mvaddch(y, x, '+');
    mvhline(y, x + 1, '=', width - 2);
    mvaddch(y, x + width - 1, '+');
}

static void draw_title(const MenuSpec *menu, const Layout *layout)
{
    int text_width = layout->title_width - BORDER_WIDTH;
    int first_text_row = layout->title_y + 1;
    int bottom_border_row = first_text_row + menu->title_lines;

    attron(A_BOLD);
    draw_horizontal_border(layout->title_y, layout->title_x, layout->title_width);
    for (int i = 0; i < menu->title_lines; ++i)
        mvprintw(first_text_row + i, layout->title_x, "| %-*s |",
                 text_width, menu->title[i]);
    draw_horizontal_border(bottom_border_row, layout->title_x, layout->title_width);
    attroff(A_BOLD);
}

static void draw_item(const MenuSpec *menu, const Layout *layout,
                      int index, int is_selected)
{
    int row = layout->items_y + index * ROWS_PER_ITEM;

    if (is_selected)
        attron(A_STANDOUT);
    mvprintw(row, layout->items_x, "| %-*s |",
             layout->label_width, menu->items[index]);
    if (is_selected)
        attroff(A_STANDOUT);
}

static void draw_too_small_message(const Layout *layout)
{
    mvprintw(0, 0, "Terminal too small (need %d columns x %d rows).",
             layout->cols_needed, layout->rows_needed);
    mvprintw(2, 0, "Resize the window, or press Q to quit.");
}

static void draw_menu(const MenuSpec *menu, const Layout *layout, int selected)
{
    erase();

    if (!layout->fits) {
        draw_too_small_message(layout);
    } else {
        draw_title(menu, layout);
        for (int i = 0; i < menu->item_count; ++i)
            draw_item(menu, layout, i, i == selected);
        if (menu->footer)
            mvaddstr(layout->footer_y, layout->footer_x, menu->footer);
    }

    refresh();
}


/* Returns the index of the item at this screen position, or -1 if none. */
static int item_at(const MenuSpec *menu, const Layout *layout, int y, int x)
{
    int row = y - layout->items_y;
    if (row < 0 || row % ROWS_PER_ITEM != 0)
        return -1;

    int index = row / ROWS_PER_ITEM;
    if (index >= menu->item_count)
        return -1;

    int is_inside_box = x >= layout->items_x
                     && x < layout->items_x + layout->items_width;
    return is_inside_box ? index : -1;
}

static int select_previous(int selected, int count)
{
    return (selected + count - 1) % count;
}

static int select_next(int selected, int count)
{
    return (selected + 1) % count;
}

static void setup_input(void)
{
    keypad(stdscr, TRUE);
    timeout(-1);
    set_escdelay(ESC_DELAY_MS);

    /* Ignore button releases, so a click can't also activate the next menu. */
    mousemask(BUTTON1_PRESSED | BUTTON1_CLICKED, NULL);
    mouseinterval(0);

    flushinp();
}

/* Returns the item under a left-click, or -1 if the event doesn't count. */
static int clicked_item(const MenuSpec *menu, const Layout *layout)
{
    MEVENT mouse;

    if (!layout->fits || getmouse(&mouse) != OK)
        return -1;
    if (!(mouse.bstate & (BUTTON1_PRESSED | BUTTON1_CLICKED)))
        return -1;

    return item_at(menu, layout, mouse.y, mouse.x);
}


int menu_run(const MenuSpec *menu, int *selected)
{
    setup_input();

    while (1) {
        Layout layout = make_layout(menu);
        draw_menu(menu, &layout, *selected);

        int key = getch();

        if (key == KEY_UP) {
            *selected = select_previous(*selected, menu->item_count);
        } else if (key == KEY_DOWN) {
            *selected = select_next(*selected, menu->item_count);
        } else if (key == '\n' || key == '\r' || key == KEY_ENTER) {
            if (layout.fits)
                return *selected;
        } else if (key == ESC_KEY) {
            return MENU_ESC;
        } else if (key == 'q' || key == 'Q') {
            return MENU_QUIT;
        } else if (key == KEY_MOUSE) {
            int item = clicked_item(menu, &layout);
            if (item >= 0) {
                *selected = item;
                return item;
            }
        }
    }
}

void menu_message(const char *text)
{
    const char *prompt = "Press any key to continue";
    int screen_rows, screen_cols;

    keypad(stdscr, TRUE);
    timeout(-1);
    flushinp();

    getmaxyx(stdscr, screen_rows, screen_cols);
    int text_x   = max_int(0, center(screen_cols, (int)strlen(text)));
    int prompt_x = max_int(0, center(screen_cols, (int)strlen(prompt)));
    int text_row = screen_rows / 2;

    erase();
    mvaddnstr(text_row,     text_x,   text,   screen_cols);
    mvaddnstr(text_row + 2, prompt_x, prompt, screen_cols);
    refresh();

    int key;
    do {
        key = getch();
    } while (key == KEY_RESIZE || key == ERR || key == KEY_MOUSE);
}