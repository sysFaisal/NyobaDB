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
            MenuProfile(win, Curent, db, ch);
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
            MenuLogout(win, Curent, db, ch);
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

    initscr();
    cbreak();  
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    int ch;

    ui_init();

    WINDOW *leftheader = derwin(left, 4, 58, 1, 1);
    WINDOW *rightheader = derwin(right, 4, 58, 1, 1);

    while(((ch = getch()) != 'q')){
        werase(left); werase(right); werase(top); werase(bottom);
        ui_draw();


        mvwprintw(top, 1, 2, "DEBUG: Tombol = %d", ch);
        
        if (ch == '\t') {
            focus = !focus;
            ch = -1;
        }

        if (focus == true) {
            UserHeader(leftheader,  &Curent1, true);
            LogicUser(left, &Curent1, db, ch);

            UserHeader(rightheader, &Curent2, false);
            LogicUser(right, &Curent2, db, -1);

        } else {
            UserHeader(rightheader, &Curent2, true);
            LogicUser(right, &Curent2, db, ch);

            UserHeader(leftheader,  &Curent1, false);
            LogicUser(left, &Curent1, db, -1);

        }
        
        wrefresh(left);
        wrefresh(right);
        wrefresh(top);
        wrefresh(bottom);

    }

    delwin(leftheader);
    delwin(rightheader);

    delwin(left);
    delwin(right);

    endwin();
    endwin();

    printf("\nTekan Apa Saja Untuk Keluar...");
    getchar();
    getchar();
    sqlite3_close(db);
    return 0;
}