#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "struct.h"
#include "ui/ui.h"
#include "c_db/sqlite3.h"
#include "setup/setup.h"
#include "login/login.h"
#include <ncurses/ncurses.h>

#define DB_NAME "db/data1.db"

void LogicUser(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch) {
    
    if (Curent->status == 0) {
        PrintList(win, db);
        LoginUser(win, Curent, db);
        
        return; 
    }
    box(win, 0, 0);

    if (ch != -1) {
        switch(ch) {
            case KEY_UP:
                if (Curent->highlight > 0) {
                    Curent->highlight--; 
                } else {
                    Curent->highlight = 4; 
                }
                break;

            case KEY_DOWN:
                if (Curent->highlight < 4) {
                    Curent->highlight++; 
                } else {
                    Curent->highlight = 0; 
                }
                break;

            case 10:          
            case KEY_ENTER:   
            
                if (Curent->highlight == 4) { 
                    Curent->status = 0; 
                    werase(win);
                }
                break;
        }
    }

    box(win, 0, 0);
    mvwprintw(win, 2, 2, "ID   : %d", Curent->id_user);
    mvwprintw(win, 3, 2, "Nama   : %s", Curent->nama);

    const char *Menu[5] = {"1. Profile", "2. Sell", "3. Buy", "4. Chat", "5. Logout"};
    
    for (int i = 0; i < 5; i++) {
        if (i == Curent->highlight) {
            wattron(win, A_REVERSE);
            mvwprintw(win, i + 6, 2, " > %s ", Menu[i]); 
            wattroff(win, A_REVERSE);
            mvwprintw(win, 19, 2, "Pilihan > %s", Menu[Curent->highlight]);
        } else {
            mvwprintw(win, i + 6, 2, "   %s ", Menu[i]); 
        }
    }


}

int main(){
    sqlite3 *db = NULL;
    LogSession Curent1 = {0};
    LogSession Curent2 = {0};

    bool focus = false;

    if(!BeginSetup(&db,DB_NAME)){
        return 0;
    }

    printf("Status : %d\n",Curent1.status);
    printf("Status : %d\n",Curent2.status);
    
    printf("\nTekan Apa Saja Untuk Masuk Ke PD Curses...");
    getchar(); getchar();

    initscr();
    cbreak();  
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    int ch;

    ui_init();

    while(((ch = getch()) != 'q')){
        werase(left); werase(right); werase(top);
        ui_draw();

        mvwprintw(top, 1, 2, "DEBUG: Tombol = %d", ch);
        
        if (ch == '\t') {
            focus = !focus;
            ch = -1;
        }

        if (focus == true) {
            mvwprintw(left, 1, 2, "Active");
            LogicUser(left, &Curent1, db, ch); 
            LogicUser(right, &Curent2, db, -1);
        } else {
            mvwprintw(right, 1, 2, "Active");
            LogicUser(left, &Curent1, db, -1);
            LogicUser(right, &Curent2, db, ch);
        }
    
        wrefresh(left);
        wrefresh(right);
        wrefresh(top);

    }

    endwin();

    printf("\nTekan Apa Saja Untuk Keluar...");
    getchar();
    getchar();
    sqlite3_close(db);
    return 0;
}