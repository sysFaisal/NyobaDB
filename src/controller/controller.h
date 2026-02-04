#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "struct.h"
#include "c_db/sqlite3.h"
#include <ncurses/ncurses.h>
#include "ui/ui.h"

void LoginUser(WINDOW *win, LogSession *Curent, sqlite3 *db);
int KeypadsInput(int c, int *highlight, int maxchoice);

void UpdateNama(WINDOW *child,LogSession *Curent, sqlite3 *db, int ch);
void UpdateSaldo(WINDOW *child, LogSession *Curent, sqlite3 *db, int ch);
void SellProduct(WINDOW *child, LogSession *Curent, sqlite3 *db, int ch);

#endif