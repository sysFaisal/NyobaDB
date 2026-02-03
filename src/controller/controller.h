#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "struct.h"
#include "c_db/sqlite3.h"
#include <ncurses/ncurses.h>

void LoginUser(WINDOW *win, LogSession *Curent, sqlite3 *db);
int KeypadsInput(int c, int *highlight, int maxchoice);

#endif