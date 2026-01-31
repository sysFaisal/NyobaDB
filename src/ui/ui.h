#ifndef UI_H
#define UI_H

#include "../PDCurses-3.9/curses.h"

// WINDOW 
extern WINDOW *top;
extern WINDOW *left;
extern WINDOW *right;

// ACTIVE WINDOW 
typedef enum {
    WIN_LEFT,
    WIN_RIGHT
} ActiveWin;

extern ActiveWin active;

// FUNCTION 
void ui_init();

#endif
