#ifndef UI_H
#define UI_H

#include <ncurses/ncurses.h>
#include "c_db/sqlite3.h"
#include "struct.h"

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
void ui_draw(int Id_user1,int Id_user2, sqlite3 *db, UserList Left[], UserList Right[]);

#endif
