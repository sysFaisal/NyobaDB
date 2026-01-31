#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "setup.h"
#include "login.h"
#include "struct.h"
#include "c_db/sqlite3.h"

//Database (H)
sqlite3* OpenDB(const char *DB_NAME){
    sqlite3 *db = NULL;

    int conn = sqlite3_open_v2(DB_NAME,&db,SQLITE_OPEN_READWRITE,NULL);

    if (conn != SQLITE_OK){
        printf("Gagal Terhubung : %s \n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    } 

    printf("Database [%s] Terhubung \n", DB_NAME);
    return db;

}

//CallBack
int Cek_CallB(void *data, int argc, char **argv, char **azColName){
    (void)argc;
    (void)argv;
    (void)azColName;

    int *found = (int *)data;
    *found = 1;
    return 0;
}

int PrintUserCallBack(void *param, int argc, char **argv, char **azColName){
    (void)azColName;
    UserData *data = (UserData *)param;
    UserList *List = data->List;

    int row = data->row;
    for (int i = 0; i < argc; i++) {
        List[row].id_user = desimal(argv[0]);
        strcpy(List[row].nama, argv[i]);
    }

    data->row++;
    return 0;
}

//Non CallBack
bool CekFile(const char *DB_NAME){
    FILE *fp = fopen(DB_NAME,"r");
    if (fp == NULL){
        return false;
    }
    fclose(fp);
    return true;
}

bool LoadUser(sqlite3 *db){
    char *errMSG = NULL;
    int found = 0;
    const char *promptsql = "SELECT 1 FROM Users LIMIT 1;";

    int conn =  sqlite3_exec(db, promptsql, Cek_CallB, &found, &errMSG);

    if (conn != SQLITE_OK){
        printf("SQLITE Error : %s\n",errMSG);
        sqlite3_free(errMSG);
        return false;
    }

    if (found == 1){
        return true;
    } else {
        return false;
        printf(" Tidak Ada User Yang Terdaftar\n");
    }
}

void Print_User(sqlite3 *db, UserList List[]){
    char *errMSG = NULL;
    UserData data = { List, 0 };
    const char *promptsql = "SELECT * FROM Users;";

    sqlite3_exec(db, promptsql, PrintUserCallBack, &data, &errMSG);
}

//Main (H)
bool BeginSetup(sqlite3 **db, const char *DB_NAME){

    if (!CekFile(DB_NAME)){
        printf("Error: File Database '%s' Tidak Ditemukan\n", DB_NAME);
        return false;
    }

    *db = OpenDB(DB_NAME);
    if (*db == NULL){
        return false;
    }

    if (LoadUser(*db)){
    } else {
        printf("Tidak ada user terdaftar\n");
    }

    return true;
}