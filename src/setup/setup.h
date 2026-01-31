#ifndef SETUP_H
#define SETUP_H
#include "c_db/sqlite3.h"

bool BeginSetup(sqlite3 **db, const char *DB_NAME);

#endif