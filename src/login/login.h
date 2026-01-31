#ifndef LOGIN_H
#define LOGIN_H
#include "struct.h"
#include "c_db/sqlite3.h"

int ProsesLogin(sqlite3 *db,int targetID, LogSession *Curent);
int desimal(char *desimal);

#endif