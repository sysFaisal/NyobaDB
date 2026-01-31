
#include "../PDCurses-3.9/curses.h"
#include "ui/ui.h"

WINDOW *top;
WINDOW *left;
WINDOW *right;

void ui_init() {
    //Maks Width CMD = 120
    //Maks Height CMD = 24
    top   = newwin(3, 120, 0, 0);
    left  = newwin(21, 60, 3, 0);
    right = newwin(21, 60, 3, 60);

    keypad(left, TRUE);
    keypad(right, TRUE);

    box(top, 0,0);
    box(left, 0, 0);
    box(right, 0, 0);
}