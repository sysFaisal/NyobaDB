#ifndef UI_H
#define UI_H

#include <ncurses/ncurses.h>
#include "c_db/sqlite3.h"
#include "struct.h"

extern WINDOW *top;
extern WINDOW *left;
extern WINDOW *right;
extern WINDOW *bottom;
 
void ui_init();
void ui_draw();
char *GetNama(LogSession *Curent, sqlite3 *db);
int GetSaldo(LogSession *Curent, sqlite3 *db);


void MenuProfile(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch);
void MenuLogout(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch);
void MenuMain(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch);
void MenuSell(WINDOW *win, LogSession *Curent, sqlite3 *db,int ch);
void UserHeader(WINDOW *header, LogSession *Curent, bool focus, sqlite3 *db);

#endif
