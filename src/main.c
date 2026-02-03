#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "controller/controller.h"
#include "struct.h"
#include "ui/ui.h"
#include "c_db/sqlite3.h"
#include "setup/setup.h"
#include "login/login.h"
#include <ncurses/ncurses.h>

#define DB_NAME "db/data1.db"

void LogicUser(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch) {

        switch (Curent->menu) {

        case MENU_MAIN:
            MenuMain(win, Curent, db, ch);
            break;

        case MENU_PROFILE:
            MenuProfile(win, Curent, ch);
            break;

        case MENU_SELL:
            //MenuSell(win, Curent, db, ch);
            break;

        case MENU_BUY:
            //MenuBuy(win, Curent, db, ch);
            break;

        case MENU_CHAT:
            //MenuChat(win, Curent, db, ch);
            break;

        case MENU_LOGOUT:
            //MenuLogout(win, Curent, db, ch);
            break;

        default:
            Curent->menu = MENU_MAIN;
            Curent->highlight = 0;
            break;
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