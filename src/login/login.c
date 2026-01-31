#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "struct.h"
#include "login.h"
#include "c_db/sqlite3.h"

// Desimal
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

// Callback
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
        strcpy(Session->nama, "");    
    }else {
        strcpy(Session->nama, argv[1]);
    }
    
    return 0;
}

// Non Callback
int ProsesLogin(sqlite3 *db,int targetID, LogSession *Curent){
    LogSession Session;
    char *errMSG = NULL;
    char Buff[100];

    Session.id_user = 0;
    Session.status = 0;
    strcpy(Session.nama, ""); 

    sprintf(Buff, "SELECT * FROM Users WHERE id_user = %d ;",targetID);
    int conn = sqlite3_exec(db,Buff,LoginCallBack,&Session, &errMSG);

    if (conn != SQLITE_OK){
        printf("SQLITE Error : %s\n",errMSG);
        sqlite3_free(errMSG);
        *Curent = Session;
        return 0;
    }

    if (Session.id_user == 0){
        printf("Login Gagal: User dengan ID %d tidak ditemukan.\n", targetID);
        *Curent = Session;
        return 0;
    }

    Session.status = 1;
    *Curent = Session;
    return 1;
}