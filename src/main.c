#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "struct.h"
#include "c_db/sqlite3.h"
#include "setup/setup.h"
#include "login/login.h"
#include "ui/ui.h"
#include "../PDCurses-3.9/curses.h"

#define DB_NAME "db/data1.db"

WINDOW *top;
WINDOW *left;
WINDOW *right;


int main(){
    sqlite3 *db = NULL;
    LogSession Curent1;
    LogSession Curent2;
    int targetID = 0;

    char buff[10];
    if(!BeginSetup(&db,DB_NAME)){
        return 0;
    }

    printf("\nTekan Apa Saja Untuk Masuk Ke PD Curses...");
    getchar(); getchar();

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    char ch;
    
    while(((ch = getch()) != 'p')){
        werase(left);
        werase(right);
        werase(top);
        ui_init();
        wrefresh(left);
        wrefresh(right);
        wrefresh(top);
    }


    /*
    while((ch = getch()) != 'q'){

        if(ch == '\t')
            active = !active;

        // 1. bersihkan
        werase(left);
        werase(right);
        werase(up);

        // 2. gambar ulang border
        box(up, 0,0);
        box(left, 0, 0);
        box(right, 0, 0);

        char *well = "PROGRAM MINI MARKET";
        int cb = 60 - (strlen(well) / 2);

        // 3. render state
        if(active == 0){
            mvwprintw(left, 1, 2, "AKTIF");
            if (Curent1.status != 1){
                mvwprintw(left, 2, 2, "Masukkan ID : ");
                wrefresh(left);
                echo();
                wgetnstr(left, buff, 9);   
                noecho();
                targetID = desimal(buff);
                ProsesLogin(db,targetID,&Curent1);    
            }
            mvwprintw(left, 2, 2, "ID User = %d",Curent1.id_user);
            mvwprintw(left, 3, 2, "Nama = %s",Curent1.nama);
            mvwprintw(up, 1, cb, "%s %d",well,Curent1.id_user);
        } else {
            mvwprintw(right, 1, 2, "AKTIF");
            if (Curent1.status != 1){
                mvwprintw(right, 2, 2, "Masukkan ID : ");
                wrefresh(right);
                echo();
                wgetnstr(right, buff, 9);   
                noecho();
                targetID = desimal(buff);
                ProsesLogin(db,targetID,&Curent2);    
            }
            mvwprintw(right, 2, 2, "ID User = %d",Curent2.id_user);
            mvwprintw(right, 3, 2, "Nama = %s",Curent2.nama);
            mvwprintw(up, 1, cb, "%s %d",well,Curent2.id_user);
        }

        // 4. refresh SEMUA
        wrefresh(left);
        wrefresh(right);
        wrefresh(up);
    }
    */
    endwin();

    printf("\nTekan Apa Saja Untuk Keluar...");
    getchar();
    getchar();
    sqlite3_close(db);
    return 0;
}