
#include <ncurses/ncurses.h>
#include <string.h>
#include "ui/ui.h"
#include "setup.h"
#include "struct.h"
#include "c_db/sqlite3.h"

WINDOW *top;
WINDOW *left;
WINDOW *right;

void ui_init() {
    //Maks Width CMD = 120
    //Maks Height CMD = 24
    top   = newwin(3, 120, 0, 0);
    left  = newwin(25, 60, 3, 0);
    right = newwin(25, 60, 3, 60);

    keypad(left, TRUE);
    keypad(right, TRUE);
    keypad(stdscr, TRUE);

    curs_set(0);

    box(top, 0,0);
    box(left, 0, 0);
    box(right, 0, 0);
}

static void draw_top(){
    box(top, 0,0);
    char *well = "PROGRAM MINI MARKET";
    int cb = 60 - (strlen(well) / 2);

    mvwprintw(top, 1, cb, "%s",well);
}

void listleft(int row, int id_user, char *nama){
    if (row == 3){
        mvwprintw(left, 2, 2, "No");
        mvwprintw(left, 2, 7, "Id_user"); 
        mvwprintw(left, 2, 20, "Nama");
    }
    mvwprintw(left, row, 2, "%d", row - 1);
    mvwprintw(left, row, 7, "%d",id_user);
    mvwprintw(left, row, 20, "%s",nama);
}

void listright(int row, int id_user, char *nama){
    if (row == 3){
        mvwprintw(right, 2, 2, "No");
        mvwprintw(right, 2, 7, "Id_user"); 
        mvwprintw(right, 2, 20, "Nama");
    }
    mvwprintw(right, row, 2, "%d", row - 1);
    mvwprintw(right, row, 7, "%d",id_user);
    mvwprintw(right, row, 20, "%s",nama);
}

static void draw_left(int Id_user1, sqlite3 *db, UserList Left[]){
    box(left, 0, 0);
    int i, row = 3;
    if (Id_user1 == 0){
        Print_User(db, Left);
        for (i = 0; i <= 1; i++){
            if (Left[i].id_user == 0) break;
            listleft(row , Left[i].id_user, Left[i].nama);
            row++;
        }
    }
}
static void draw_right(int Id_user2, sqlite3 *db, UserList Right[]){
    box(right, 0, 0);
    int i, row = 3;
    if (Id_user2 == 0) {
        Print_User(db, Right);
        for (i = 0; i <= 1; i++){
            if (Right[i].id_user == 0) break;
            listright(row , Right[i].id_user, Right[i].nama);
            row++;
        }
    }
}

void ui_draw(int Id_user1,int Id_user2, sqlite3 *db, UserList Left[], UserList Right[]){
    draw_top();
    draw_left(Id_user1, db, Left);
    draw_right(Id_user2, db, Right);
}