#ifndef SETUP_H
#define SETUP_H
#include "c_db/sqlite3.h"
#include "struct.h"

bool BeginSetup(sqlite3 **db, const char *DB_NAME);
void Print_User(sqlite3 *db, UserList List[]);

#endif