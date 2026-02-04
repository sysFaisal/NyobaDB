
#include <ncurses/ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "controller/controller.h"
#include "ui/ui.h"
#include "struct.h"

WINDOW *top;
WINDOW *left;
WINDOW *right;
WINDOW *bottom;


void ui_init() {
    //Maks Width CMD = 120
    //Maks Height CMD = 24
    top   = newwin(3, 120, 0, 0);
    left  = newwin(24, 60, 3, 0);
    right = newwin(24, 60, 3, 60);
    bottom = newwin(3, 120, 27, 0);

    keypad(left, TRUE);
    keypad(right, TRUE);
    keypad(stdscr, TRUE);

    curs_set(0);

    box(top, 0,0);
    box(left, 0, 0);
    box(right, 0, 0);
    box(bottom, 0, 0);
}

static void draw_top(){
    box(top, 0,0);
    char *well = "PROGRAM MINI MARKET";
    int cb = 60 - (strlen(well) / 2);

    mvwprintw(top, 1, cb, "%s",well);
}

static void draw_left(){
    box(left, 0, 0);
}

static void draw_right(){
    box(right, 0, 0);
}

static void draw_bottom(){
    box(bottom, 0, 0);
    int width = 120 - 2;  
    int start = 1;         

    char *nav   = "[UP/DOWN] Navigasi";
    char *back  = "[B] Back";
    char *enter = "[ENTER] Choice";
    char *quit  = "[Q] Exit";

    int slot = width / 4;
    int y = 1;

    mvwprintw(bottom, y,
        start + (slot * 0) + (slot - strlen(nav)) / 2,
        "%s", nav);

    mvwprintw(bottom, y,
        start + (slot * 1) + (slot - strlen(back)) / 2,
        "%s", back);

    mvwprintw(bottom, y,
        start + (slot * 2) + (slot - strlen(enter)) / 2,
        "%s", enter);

    mvwprintw(bottom, y,
        start + (slot * 3) + (slot - strlen(quit)) / 2,
        "%s", quit);
}

void ui_draw(){
    draw_top();
    draw_left();
    draw_right();
    draw_bottom();
}


// GET SALDO =========================================================================
int GetSaldoCallBack(void *data, int argc, char **argv, char **azColName){
    (void)azColName;
    (void)argc;

    int *test = (int *)data;
        if (argv[0]) {
        *test = atoi(argv[0]);
    }
    return 0;
}

int GetSaldo(LogSession *Curent, sqlite3 *db){
    char buff[128]; int test = 0;

    sprintf(buff, "SELECT saldo FROM Wallet WHERE id_user = %d;", Curent->id_user);
    sqlite3_exec(db, buff, GetSaldoCallBack, &test, NULL);

    return test;
}
//END CODE =========================================================================


// MAIN MENU =========================================================================
void MenuMain(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch){
    WINDOW *child = derwin(win, 18, 58, 5, 1);


    if (Curent->status == 0) {
        LoginUser(win, Curent, db);
        wrefresh(win);
        return;
    }

    werase(child);
    box(child, 0, 0);

    const char *Menu[5] = {
        "Profile", "Sell", "Buy", "Chat", "Logout"
    };

    int choice = -1;
    if (ch != -1) {
        choice = KeypadsInput(ch, &Curent->highlight[0], 5);
    }

    for (int i = 0; i < 5; i++) {
        if (i == Curent->highlight[0]) {
            wattron(child, A_REVERSE);
            mvwprintw(child, i + 2, 2, "> %s", Menu[i]);
            wattroff(child, A_REVERSE);
        } else {
            mvwprintw(child, i + 2, 2, "  %s", Menu[i]);
        }
    }

    wrefresh(child);


    if (choice == 1) {
        Curent->level = 1;          // NAIK LEVEL
        Curent->highlight[1] = 0;   // RESET SUBMENU

        switch (Curent->highlight[0]) {
            case 0: Curent->menu = MENU_PROFILE; break;
            case 1: Curent->menu = MENU_SELL; break;
            case 2: Curent->menu = MENU_BUY; break;
            case 3: Curent->menu = MENU_CHAT; break;
            case 4: Curent->menu = MENU_LOGOUT; break;
        }
        wrefresh(child);
        delwin(child);
    }
}
// END CODE =========================================================================


// PROFILE MENU =========================================================================
void ProfilePilihan(WINDOW *child, LogSession *Curent, sqlite3 *db, int ch){
    const char *Pilihan[2] = {
        "Update Saldo",
        "Update Nama"
    };

    int choice = -1;
    if (ch != -1) {
        choice = KeypadsInput(ch, &Curent->highlight[1], 2);
    }

    int row = 7;
    for (int i = 0; i < 2; i++) {
        if (i == Curent->highlight[1]) {
            wattron(child, A_REVERSE);
            mvwprintw(child, row + i, 2, "> %s", Pilihan[i]);
            wattroff(child, A_REVERSE);
        } else {
            mvwprintw(child, row + i, 2, "  %s", Pilihan[i]);
        }
    }

    mvwprintw(child, 16, 2, "%d", Curent->highlight[1]);
    if (choice == 1) {
        switch (Curent->highlight[1]) {
            case 0: UpdateSaldo(child, Curent, db, ch); break;
            case 1: //UpdateNama(child, Curent, db, ch); break;
        }
    }
}

void MenuProfile(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch){
    static WINDOW *child = NULL;
    static WINDOW *foto  = NULL;

    if (!child) {
        child = derwin(win, 18, 58, 5, 1);
        foto  = derwin(child, 5, 9, 1, 2);
    }

    werase(child);
    werase(foto);
    box(child, 0, 0);
    box(foto, 0, 0);

    GetSaldo(Curent, db);
    mvwprintw(child, 2, 12, "ID User : %d", Curent->id_user);
    mvwprintw(child, 3, 12, "Nama    : %s", Curent->nama);
    mvwprintw(child, 4, 12, "Saldo   : %d", GetSaldo(Curent, db));

    ProfilePilihan(child, Curent, db, ch);

    if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->level = 0;

        delwin(foto);
        delwin(child);
        foto = NULL;
        child = NULL;
    }

    wrefresh(foto);
    wrefresh(child);
}
// END CODE =========================================================================


// HEADER ========================================================================
void UserHeader(WINDOW *header, LogSession *Curent, bool focus){
    box(header, 0, 0);

    mvwprintw(header, 15, 2, "ID   : %d", Curent->menu);
    if (Curent->status != 0) {
        mvwprintw(header, 1, 2, "ID   : %d", Curent->id_user);
        mvwprintw(header, 2, 2, "Nama   : %s", Curent->nama);

        if (focus == true) {
            const char *Panel = " Panel User : Active ";
            int panjang = (58 - strlen(Panel)) / 2;
            mvwprintw(header, 0, panjang , " %s", Panel);
        } else {
            const char *Panel1 = " Panel User : Inactive ";
            int panjang1 = (58 - strlen(Panel1)) / 2;
            mvwprintw(header, 0, panjang1, "%s", Panel1);
        }
    } else {
        mvwprintw(header, 1, 24, "L O G I N");
    }
}
//END CODE ==============================================================


// SELL MENU =========================================================================
typedef struct {
    int count;
    LogSession *Curent;
    WINDOW *child;
    int *call_pos;
} ViewSell;

int ListProductCallBack(void *param, int argc, char **argv, char **azColName){
    (void)azColName;
    (void)argc;

    ViewSell *temp = (ViewSell *)param;
    int row = temp->count;

    mvwprintw(temp->child, row, temp->call_pos[0], "You");
    mvwprintw(temp->child, row, temp->call_pos[1], "%d", atoi(argv[1]));
    mvwprintw(temp->child, row, temp->call_pos[2], "%s", argv[2]);
    mvwprintw(temp->child, row, temp->call_pos[3] + 1, "%d", atoi(argv[3]));
    mvwprintw(temp->child, row, temp->call_pos[4], "%d", atoi(argv[4]));

    temp->count++;
    return 0;
} 

void ListProduct(WINDOW *child, LogSession *Curent, sqlite3 *db,int ch){

    char Buff[100]; 
    char *errMsg = NULL;

    int col_pos[] = {2, 7, 19, 41, 48};

    sprintf(Buff, "SELECT * FROM Product WHERE id_user = %d ;", Curent->id_user);

    ViewSell temp;
    temp.count = 4;
    temp.child = child;
    temp.Curent = Curent;
    temp.call_pos = col_pos;

    int rc = sqlite3_exec(db, Buff, ListProductCallBack, &temp, &errMsg);

    if (rc != SQLITE_OK) {
        mvwprintw(child, 5, 2, "SQL Error: %s", errMsg);
        sqlite3_free(errMsg);

    } else if (temp.count == 4) {
        mvwprintw(child, 4, 2, "Data Kosong / Tidak Ditemukan");
    }


}

void SellPilihan(WINDOW *child, LogSession *Curent, sqlite3 *db,int ch){
    const char *Pilihan[2] = {
        "Update Product", 
        "Sell Product"
    };

    int choice = -1;
    if (ch != -1) {
        choice = KeypadsInput(ch, &Curent->highlight[1], 2);
    }

    if (choice == 1) {
        switch (Curent->highlight[1]) {
            case 0: mvwprintw(child, 15, 2, "Pilihan > %s", Pilihan[Curent->highlight[1]]); break;
            case 1: SellProduct(child, Curent, db, ch); break;
        }
    }

    int i, row = 7;
    for (i = 0; i < 2; i++){
        if (i == Curent->highlight[1]){
            wattron(child, A_REVERSE);
            mvwprintw(child, row + i, 2, "> %s", Pilihan[i]);
            wattroff(child, A_REVERSE);
        } else {
            mvwprintw(child, row + i, 2, "> %s", Pilihan[i]);
        }
    }

    mvwprintw(child, 16, 2, "[DEBUG] Curent %d", Curent->highlight[1]);

    if (Curent->highlight[1] >= 0 && Curent->highlight[1] < 2) {
        mvwprintw(child, 15, 2, "Choice > %s", Pilihan[Curent->highlight[1]]);

    }

    if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->level = 0;

    }

}

void MenuSell(WINDOW *win, LogSession *Curent, sqlite3 *db,int ch){

    static WINDOW *child = NULL;

    if (!child) {
        child = derwin(win, 18, 58, 5, 1);
    }
    
    werase(child);
    box (child, 0, 0);

    mvwprintw(child, 1 , 2, "%s", "PRODUCT");
    mvwprintw(child, 3 , 2, "%s", "ID");
    mvwprintw(child, 3 , 7, "%s", "ID Product");
    mvwprintw(child, 3 , 19, "%s", "Name");
    mvwprintw(child, 3 , 41, "%s", "Qty");
    mvwprintw(child, 3 , 48, "%s", "Cost");

    ListProduct(child, Curent, db, ch);
    SellPilihan(child, Curent, db, ch);

    wrefresh(child);
    if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->level = 0;

        delwin(child);
        child = NULL;
    }
}
// END CODE =========================================================================
