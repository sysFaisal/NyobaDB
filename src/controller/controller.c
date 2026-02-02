#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../src/struct.h"
#include "controller.h"
#include "login/login.h"
#include "ui/ui.h"
#include <ncurses/ncurses.h>

typedef struct {
    int id_user;
    char nama[100];
} UserList;

typedef struct {
    int count;
    UserList *list;
} ViewContext;

//Callback
int PrintUserCallBack(void *param, int argc, char **argv, char **azColName){
    (void)azColName;
    (void)argc;

    ViewContext *temp = (ViewContext *)param;
 
    temp->list[temp->count].id_user = atoi(argv[0]);
    strcpy(temp->list[temp->count].nama, argv[1]);

    temp->count++;
    return 0;

}
    
//Non CallBack
int PrintList(WINDOW *win, sqlite3 *db){
    UserList List[20] = {0};

    werase(win);
    box(win, 0, 0);

    ViewContext temp;
    temp.count = 0;
    temp.list = List;

    const char *promptsql = "SELECT * FROM Users;";

    sqlite3_exec(db, promptsql, PrintUserCallBack, &temp, NULL);

    if (temp.count == 0) {
        mvwprintw(win, 2, 2, "Tidak ada data user!");
        wrefresh(win);
        return -1;
    }

    int highlight = 0; int choice = -1;
    int c; int i, row = 3;

    keypad(win, TRUE);

    while(choice == -1){
        c = wgetch(win);

        mvwprintw(win, 19, 2, "Pilihan ID : %d", temp.list[highlight].id_user);
        wrefresh(win);

        switch(c){
            case KEY_UP:
                if (highlight > 0) {
                    highlight--; 
                } else {
                    highlight = temp.count - 1; 
                }
                break;

            case KEY_DOWN:
                if (highlight < temp.count - 1) {
                    highlight++; 
                } else {
                    highlight = 0; 
                }
                break;

            case 10:          
            case KEY_ENTER:   
                choice = temp.list[highlight].id_user;
                break;
        }

        for (i = 0; i < temp.count; i++) {
            if (i == highlight){
                wattron(win, A_REVERSE);
                mvwprintw(win, row + i, 2, "%d", i + 1);
                mvwprintw(win, row + i, 7, "%d", temp.list[i].id_user);
                mvwprintw(win, row + i, 20, "%s", temp.list[i].nama);
                wattroff(win, A_REVERSE);
            } else {
                mvwprintw(win, row + i, 2, "%d", i + 1);
                mvwprintw(win, row + i, 7, "%d", temp.list[i].id_user);
                mvwprintw(win, row + i, 20, "%s", temp.list[i].nama);
            }
            
        }
    }
    return choice;
}

void LoginUser(WINDOW *win, LogSession *Curent, sqlite3 *db){

    int ChoiceId = PrintList(win ,db);

    if (ChoiceId != -1) {
        ProsesLogin(db, ChoiceId, Curent);
        Curent->highlight = 0;
    } else {

    }

    werase(win);
    box(win, 0, 0);
}

