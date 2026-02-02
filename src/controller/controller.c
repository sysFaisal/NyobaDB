#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "src/struct.h"
#include "controller.h"
#include "login/login.h"
#include "ui/ui.h"
#include <ncurses/ncurses.h>

typedef struct {
    WINDOW *win;
    int row;
} ViewContext;

//Callback
int PrintUserCallBack(void *param, int argc, char **argv, char **azColName){
    (void)azColName;

    ViewContext *temp = (ViewContext *)param;
    WINDOW *win = ViewContext->win;

    int row = temp->row;
    for (int i = 0; i < argc; i++) {
        mvwprintw(temp->win, row, 2, "%d", row - 2);
        mvwprintw(temp->win, row, 7, "%d",atoi(argv[0]));
        mvwprintw(temp->win, row, 20, "%s",argv[i]);
    }
    temp->row++;
    return 0;

}
    

void PrintList(WINDOW *win, sqlite3 *db){

    werase(win);
    box(win, 0, 0);

    ViewContext temp;
    temp.win = win;
    temp.row = 3;

    mvwprintw(right, 2, 2, "No");
    mvwprintw(right, 2, 7, "Id_user"); 
    mvwprintw(right, 2, 20, "Nama");

    const char *promptsql = "SELECT * FROM Users;";

    sqlite3_exec(db, promptsql, PrintUserCallBack, &temp, NULL);

    wrefresh(win);
}

void LoginUser(WINDOW *win, LogSession *Curent, sqlite3 *db){
    char buff[10];

    mvwprintw(win, 19, 2, "Masukan ID : ");
    echo(); curs_set(1);
    mvwgetnstr(win, 19, 15, buff, 9); 
    noecho(); curs_set(0);
    
    ProsesLogin(db, atoi(buff), Curent);
    Curent->highlight = 0;
     
    werase(win);
}