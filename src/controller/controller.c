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
int KeypadsInput(int c, int *highlight, int maxchoice){

    switch(c){
        case KEY_UP:
            if (*highlight > 0) {
                *highlight = *highlight - 1;
            } else {
                *highlight = maxchoice - 1; 
            }
            break;

        case KEY_DOWN:
            if (*highlight < maxchoice - 1) {
                *highlight = *highlight + 1; 
            } else {
                *highlight = 0; 
            }
            break;

        case 10:          
        case KEY_ENTER:   
            return 1;   
    }
    return -1;

}

int PrintList(WINDOW *win, sqlite3 *db){
    UserList List[20] = {0};

    //werase(win);
    box(win, 0, 0);

    WINDOW *child = derwin(win, 18, 58, 5, 1); box(child, 0, 0);

    ViewContext temp;
    temp.count = 0;
    temp.list = List;

    const char *promptsql = "SELECT * FROM Users;";

    sqlite3_exec(db, promptsql, PrintUserCallBack, &temp, NULL);

    if (temp.count == 0) {
        mvwprintw(child, 2, 2, "Tidak ada data user!");
        wrefresh(child);
        return -1;
    }

    int highlight = 0; int choice = -1;
    int c; int i, row = 2;

    keypad(win, TRUE);
    keypad(child, TRUE);

    while(choice == -1){

        c = wgetch(child);

        mvwprintw(child, 1 , 2, "%s", "No");
        mvwprintw(child, 1 , 7, "%s", "Id_user");
        mvwprintw(child, 1 , 20, "%s", "Nama");

        wrefresh(child);

        int Select = KeypadsInput(c, &highlight, temp.count);

        if (Select != -1){
            choice = temp.list[highlight].id_user;  
        }

        mvwprintw(child, 16, 2, "[DEBUG] Curent : %d", highlight);
        mvwprintw(child, 15, 2, "Pilihan > %d", temp.list[highlight].id_user);
        
        for (i = 0; i < temp.count; i++) {
            if (i == highlight){
                wattron(child, A_REVERSE);

                mvwprintw(child, row + i, 2, "%d", i + 1);
                mvwprintw(child, row + i, 7, "%d", temp.list[i].id_user);
                mvwprintw(child, row + i, 20, "%s", temp.list[i].nama);

                wattroff(child, A_REVERSE);
            } else {

                mvwprintw(child, row + i, 2, "%d", i + 1);
                mvwprintw(child, row + i, 7, "%d", temp.list[i].id_user);
                mvwprintw(child, row + i, 20, "%s", temp.list[i].nama);

            }
        }
    }
    
    delwin(child);
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


void FixLogout(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch){
    (void)db;
    (void)ch;

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 2, 2, "Logging out...");
    wrefresh(win);

    napms(500);

    Curent->status    = 0;
    Curent->id_user   = 0;
    Curent->highlight = 0;
    Curent->menu = MENU_MAIN;
}

void MenuLogout(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch){
    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 2, 2, "LOG OUT");
    mvwprintw(win, 5, 2, "[ENTER] Logout");
    mvwprintw(win, 4, 2, "[B] Back");

    if (ch == '\n' || ch == KEY_ENTER) {
        FixLogout(win, Curent, db, ch);
    } else if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->highlight = 0;
}


}

void UpdateSaldo(LogSession *Curent, sqlite3 *db){
    /*
    char Buff[100];
    sprintf(Buff, "UPDATE Users SET saldo = saldo + 100 WHERE id_user = %d ;", Curent->id_user);
    sqlite3_exec(db, Buff, NULL, NULL, NULL);
    */
}

void UpdateNama(LogSession *Curent, sqlite3 *db){
    /*
    char Buff[100];
    sprintf(Buff, "UPDATE Users SET nama = '%s' WHERE id_user = %d ;", Curent->nama, Curent->id_user);
    sqlite3_exec(db, Buff, NULL, NULL, NULL);
    */
}
