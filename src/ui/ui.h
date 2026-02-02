#ifndef UI_H
#define UI_H

#include <ncurses/ncurses.h>
#include "c_db/sqlite3.h"
#include "struct.h"

extern WINDOW *top;
extern WINDOW *left;
extern WINDOW *right;
 
void ui_init();
void ui_draw();

#endif
