/*
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "c_db/sqlite3.h"
#include "curses.h"

#define MAX_NAME 100
#define DB_NAME "db/data1.db"

typedef struct{
    int id_user;
    char nama[MAX_NAME];
}LogSession;

int desimal(char *desimal){
    int panjang = strlen(desimal);
    int pangkat = 1;
    int hasil = 0; 
    int i;

    for (i = panjang - 1; i >= 0; i--) {
		hasil = hasil + (desimal[i] - '0') * pangkat;
        pangkat = pangkat * 10;
    }

    return hasil;
}

bool CekFile(const char *filename){
    FILE *fp = fopen(filename,"r");
    if (fp == NULL){
        return false;
    }
    fclose(fp);
    return true;
}

int Cek_CallB(void *data, int argc, char **argv, char **azColName){
    (void)argc;
    (void)argv;
    (void)azColName;

    int *found = (int *)data;
    *found = 1;
    return 0;
}

int PrintUserCallBack(void *data, int argc, char **argv, char **azColName){
    int *row = (int *)data;

    if (*row == 0) {
        printf("No ");
        for (int i = 0; i < argc; i++) {
            printf("| %-10s ", azColName[i]);
        }
        printf("\n");

        printf("---");
        for (int i = 0; i < argc; i++) {
            printf("+------------");
        }
        printf("\n");
    }

    (*row)++;
    printf("%-2d ", *row);

    for (int i = 0; i < argc; i++) {
        printf("| %-10s ", argv[i] ? argv[i] : "NULL");
    }
    printf("\n");

    return 0;
}

int LoginCallBack(void *data, int argc, char **argv, char **azColName){
    (void)argc;
    (void)azColName;
    LogSession *Session = (LogSession *)data;

    if ( argv[0] == NULL){
        Session->id_user = 0; 
    } else {
        Session->id_user = desimal(argv[0]);
    }

    if( argv[1] == NULL){
        strcpy(Session->nama, "Tidak Ada");    
    }else {
        strcpy(Session->nama, argv[1]);
    }
    
    return 0;
}

bool CekUser(sqlite3 *db){
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
    }
    
}

void PrintAllUser(sqlite3 *db){
    char *errMSG = NULL;
    int row = 0;
    const char *promptsql = "SELECT * FROM Users";

    int conn =  sqlite3_exec(db, promptsql, PrintUserCallBack, &row, &errMSG);

    if (conn != SQLITE_OK){
        printf("SQLITE Error : %s\n",errMSG);
        sqlite3_free(errMSG);
    }    
}

int ProsesLogin(sqlite3 *db, int Target, LogSession *Curent){
    LogSession Session;
    char *errMSG = NULL;
    char Buff[256];

    Session.id_user = -1;
    strcpy(Session.nama, "");

    sprintf(Buff, "SELECT * FROM Users WHERE id_user = %d ;",Target);

    int conn =  sqlite3_exec(db, Buff, LoginCallBack, &Session, &errMSG);

    if (conn != SQLITE_OK){
        printf("SQLITE Error : %s\n",errMSG);
        sqlite3_free(errMSG);
        *Curent = Session;
        return 0;
    }

    if (Session.id_user == -1) {
        printf("Login Gagal: User dengan ID %d tidak ditemukan.\n", Target);
        *Curent = Session;
        return 0;
    } else {
        printf("Login berhasil\n");
        *Curent = Session;
        return 1;
    }


}

int main (){
    sqlite3 *db;
    LogSession Curent = {0} ;
    int targetID = 0;
    
    if (!CekFile(DB_NAME)) {
        printf("Error : File Databases '%s' Tidak Ditemukan",DB_NAME);
        return -1; 
    }

    int conn = sqlite3_open_v2(DB_NAME,&db,SQLITE_OPEN_READWRITE,NULL);
    if (conn != SQLITE_OK){
        printf("Gagal Terhubung : %s ",sqlite3_errmsg(db));
        return -1;
    }

    if (!CekUser(db)){
        printf("Belum ada User terdaftar\n");
    } else {
        printf("========= Daftar User ========\n");
        PrintAllUser(db);
    }

    printf("Masukan Id_User yang Ingin dipilih : ");
    if (scanf("%d",&targetID) == 1){
        ProsesLogin(db,targetID,&Curent);
    } else {
        printf("Input Tidak Valid\n");
    }

    printf("1. ID   : %d\n", Curent.id_user);
    printf("2. Nama : %s\n", Curent.nama);

    printf("\nTekan ENTER untuk keluar...");
    getchar();
    getchar();

    sqlite3_close(db);
    return 0;
}
*/