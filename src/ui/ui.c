
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


// GET PRODUCT =======================================================================
int GetProductCallBack(void *data, int argc, char **argv, char **azColName){
    (void)azColName;
    (void)argc;
    (void)argv;

    int *test = (int *)data;
    *test = *test + 1;

    return 0;
}

int GetProduct(LogSession *Curent, sqlite3 *db){
    char buff[128];
    int MyProduct = 0;
    sprintf(buff,
        "SELECT * FROM Product WHERE id_user = %d;",
        Curent->id_user
    );

    sqlite3_exec(db, buff, GetProductCallBack, &MyProduct, NULL);
    return  MyProduct;
}

// END CODE =========================================================================

// GET NAMA =========================================================================
int GetNamaCallBack(void *data, int argc, char **argv, char **azColName){
    (void)azColName;
    (void)argc;

    char *buf = (char *)data;

    if (argv[0]) {
        strcpy(buf, argv[0]);
    }
    return 0;
}

char *GetNama(LogSession *Curent, sqlite3 *db){
    static char nama[64]; 
    nama[0] = '\0';

    char buff[128];
    sprintf(buff,
        "SELECT Nama FROM Users WHERE id_user = %d;",
        Curent->id_user
    );

    sqlite3_exec(db, buff, GetNamaCallBack, nama, NULL);
    return nama;
}
// END CODE =========================================================================


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
    mvwprintw(child, 16, 2, "> %s", Menu[Curent->highlight[0]]);
    mvwprintw(child, 16, 55, "%d", Curent->highlight[0]);

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

    int row = 12;
    for (int i = 0; i < 2; i++) {
        if (i == Curent->highlight[1]) {
            wattron(child, A_REVERSE);
            mvwprintw(child, row + i, 2, "> %s", Pilihan[i]);
            wattroff(child, A_REVERSE);
        } else {
            mvwprintw(child, row + i, 2, "  %s", Pilihan[i]);
        }
    }

    mvwprintw(child, 16, 55, "%d", Curent->highlight[1]);
    mvwprintw(child, 16, 2, "> %s", Pilihan[Curent->highlight[1]]);

    if (choice == 1) {
        switch (Curent->highlight[1]) {
            case 0: UpdateSaldo(child, Curent, db, ch); break;
            case 1: UpdateNama(child, Curent, db, ch); break;
        }
    }
}

void MenuProfile(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch){
    WINDOW *child = derwin(win, 18, 58, 5, 1);
    WINDOW *foto  = derwin(child, 5, 9, 1, 2);

    /*
    if (!child) {
        child = derwin(win, 18, 58, 5, 1);
        foto  = derwin(child, 5, 9, 1, 2);
    }
    */
    werase(child);
    werase(foto);

    box(child, 0, 0);
    box(foto, 0, 0);
    
    mvwprintw(child, 2, 12, "ID User : %d", Curent->id_user);
    mvwprintw(child, 3, 12, "Nama    : %s", GetNama(Curent, db));
    mvwprintw(child, 4, 12, "Saldo   : %d", GetSaldo(Curent, db));

    ProfilePilihan(child, Curent, db, ch);


    if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->level = 0;

    }

    wrefresh(foto);
    wrefresh(child);

    if (Curent->menu != MENU_PROFILE) {
        delwin(foto);
        delwin(child);
    }
  
}
// END CODE =========================================================================


// HEADER ========================================================================
void UserHeader(WINDOW *header, LogSession *Curent, bool focus, sqlite3 *db){
    box(header, 0, 0);
    char buff[128] = ""; char buff1[40] = "";

    mvwprintw(header, 15, 2, "ID   : %d", Curent->menu);
    if (Curent->status != 0) {
        mvwprintw(header, 1, 2, "ID   : %d", Curent->id_user);
        mvwprintw(header, 2, 2, "Name   : %s", GetNama(Curent, db));

        sprintf(buff1, "Product Total : %d", GetProduct(Curent, db));
        sprintf(buff, "Balance : %d", GetSaldo(Curent, db));

        mvwprintw(header, 2, 58 - 2 - strlen(buff1), "%s", buff1);
        mvwprintw(header, 1, 58 - 2 - strlen(buff), "%s", buff);

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
        werase(header);
        box(header, 0, 0);
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

typedef struct {
    int count;
    LogSession *Curent;
    WINDOW *child;
    int *call_pos;
} ViewBuyer;

int ListProductCallBack(void *param, int argc, char **argv, char **azColName){
    (void)azColName;
    (void)argc;

    ViewSell *temp = (ViewSell *)param;
    int row = temp->count;

    
    mvwprintw(temp->child, row, temp->call_pos[0], "You");
    mvwprintw(temp->child, row, temp->call_pos[1], "%d", atoi(argv[1]));
    mvwprintw(temp->child, row, temp->call_pos[2], "%s", argv[2]);
    if (atoi(argv[3]) <= 0) {
        mvwprintw(temp->child, row, temp->call_pos[3], "S%dld", atoi(argv[3]));
    } else {
        mvwprintw(temp->child, row, temp->call_pos[3], "%d", atoi(argv[3]));
    }
    mvwprintw(temp->child, row, temp->call_pos[4], "%d", atoi(argv[4]));

    temp->count++;
    return 0;
} 

void ListProduct(WINDOW *child, LogSession *Curent, sqlite3 *db,int ch){
    (void)ch;

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

int GetBuyerCallBack(void *data, int argc, char **argv, char **azColName){
    (void)azColName;
    
    ViewBuyer *temp = (ViewBuyer *)data;
    int row = temp->count;

    mvwprintw(temp->child, row, temp->call_pos[0], "%s", argv[2]);
    mvwprintw(temp->child, row, temp->call_pos[1], "%s", argv[1]);
    mvwprintw(temp->child, row, temp->call_pos[2], "%s", argv[0]);
    mvwprintw(temp->child, row, temp->call_pos[3], "%s", argv[4]);
    mvwprintw(temp->child, row, temp->call_pos[4], "%s", argv[5]);
    mvwprintw(temp->child, row, temp->call_pos[5], "%.15s", argv[6]);

    temp->count++;
    return 0;
}

void CekBuyer(WINDOW *child, LogSession *Curent, sqlite3 *db,int ch){

    char buff[256]; char *errMsg = NULL; int col_pos[] = {2, 7, 15, 24, 29, 39};
    sprintf(buff, "SELECT * FROM Transaksi WHERE id_seller = %d", Curent->id_user);
    
    ViewBuyer temp;
    temp.count = 2;
    temp.child = child;
    temp.Curent = Curent;
    temp.call_pos = col_pos;

    werase(child);
    box(child, 0, 0);

    mvwprintw(child, 1, col_pos[0], "ID ");
    mvwprintw(child, 1, col_pos[1], "Buyyer");
    mvwprintw(child, 1, col_pos[2], "IdTranc");
    mvwprintw(child, 1, col_pos[3], "Qty");
    mvwprintw(child, 1, col_pos[4], "Total");
    mvwprintw(child, 1, col_pos[5], "Name Product");

    int rc = sqlite3_exec(db, buff, GetBuyerCallBack, &temp, &errMsg);

    if (rc != SQLITE_OK) {
        mvwprintw(child, 15, 2, "SQL Error: %s", errMsg);
        sqlite3_free(errMsg);
        return;
    }

    if (temp.count == 2) {
        mvwprintw(child, 2, 2, "Tidak ada data user!");
        wrefresh(child);
        return;
    }
}

void SellPilihan(WINDOW *child, LogSession *Curent, sqlite3 *db,int ch){
    const char *Pilihan[2] = {
        "Cek Siapa Aja Beli", 
        "Sell Product"
    };

    int choice = -1;
    if (ch != -1) {
        choice = KeypadsInput(ch, &Curent->highlight[1], 2);
    }

    if (choice == 1) {
        switch (Curent->highlight[1]) {
            case 0: CekBuyer(child, Curent, db, ch); break;
            case 1: SellProduct(child, Curent, db, ch); break;
        }
    }

    int i, row = 12;
    for (i = 0; i < 2; i++){
        if (i == Curent->highlight[1]){
            wattron(child, A_REVERSE);
            mvwprintw(child, row + i, 2, "> %s", Pilihan[i]);
            wattroff(child, A_REVERSE);
        } else {
            mvwprintw(child, row + i, 2, "> %s", Pilihan[i]);
        }
    }

    mvwprintw(child, 16, 55, "%d", Curent->highlight[1]);

    if (Curent->highlight[1] >= 0 && Curent->highlight[1] < 2) {
        mvwprintw(child, 16, 2, "> %s", Pilihan[Curent->highlight[1]]);

    }

    if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->level = 0;

    }

}

void MenuSell(WINDOW *win, LogSession *Curent, sqlite3 *db,int ch){

    WINDOW *child = derwin(win, 18, 58, 5, 1);
    
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
    }

    if (Curent->menu != MENU_SELL) {
        delwin(child);
    }
}
// END CODE =========================================================================


// BUY MENU =========================================================================
typedef struct {
    int id_user;
    int id_product;
    char nama[100];
    int jumlah;
    int harga;
} AllProduct;

typedef struct {
    int count;
    int index;
    LogSession *Curent;
    WINDOW *child;
    int *call_pos;
    AllProduct *prod;
} ViewProduct;

int AllListProductCallBack(void *param, int argc, char **argv, char **azColName){
    (void)azColName;
    (void)argc;

    ViewProduct *temp = (ViewProduct *)param;

    if (atoi(argv[3]) > 0) {
        temp->prod[temp->index].id_user = atoi(argv[0]);
        temp->prod[temp->index].id_product = atoi(argv[1]);
        strcpy(temp->prod[temp->index].nama, argv[2]);
        temp->prod[temp->index].jumlah = atoi(argv[3]);
        temp->prod[temp->index].harga = atoi(argv[4]);

        temp->index++;
        temp->count++;
    }

    return 0;
}

void AllListProduct(WINDOW *child, LogSession *Curent, sqlite3 *db,int ch){
    (void)ch;
    AllProduct arr[40] = {0};

    char *errMsg = NULL;

    int col_pos[] = {2, 9, 19, 41, 48};

    const char *Prompt = "SELECT * FROM Product;";

    ViewProduct temp;
    temp.prod = arr;
    temp.index = 0;
    temp.child = child;
    temp.Curent = Curent;
    temp.call_pos = col_pos;

    int rc = sqlite3_exec(db, Prompt, AllListProductCallBack, &temp, &errMsg);
    int choce = -1;

    if (ch != -1) {
        choce = KeypadsInput(ch, &Curent->highlight[1], temp.index);
    }

    if (choce == 1) {

        if (temp.prod[Curent->highlight[1]].id_user != Curent->id_user) {

            AllProduct *p = &temp.prod[Curent->highlight[1]];

            mvwprintw(child, 15, 2, "Pilihan > %d",temp.prod[Curent->highlight[1]].id_product);
            BuyProduct(child, Curent, db, p->id_user, p->id_product, p->jumlah, p->harga ,p->nama);

        } else {
            mvwprintw(child, 16, 2, "Pilihan > Barang Punya Lu");
        }

    }

    int row1 = 4;
    mvwprintw(child, 16, 47, "%d : %d : %d", Curent->highlight[1], temp.prod[Curent->highlight[1]].id_user, temp.prod[Curent->highlight[1]].id_product);

    for (int i = 0; i < temp.index; i++) {
        int is_selected = (i == Curent->highlight[1]);
        int is_me = (temp.prod[i].id_user == Curent->id_user);

        if (is_selected)
            wattron(child, A_REVERSE);

        if (is_me)
            mvwprintw(child, row1, temp.call_pos[0], "You");
        else
            mvwprintw(child, row1, temp.call_pos[0], "%d", temp.prod[i].id_user);

        mvwprintw(child, row1, temp.call_pos[1], "%d", temp.prod[i].id_product);
        mvwprintw(child, row1, temp.call_pos[2], "%s", temp.prod[i].nama);
        mvwprintw(child, row1, temp.call_pos[3], "%d", temp.prod[i].jumlah);
        mvwprintw(child, row1, temp.call_pos[4], "%d", temp.prod[i].harga);

        if (is_selected)
            wattroff(child, A_REVERSE);

        row1++;
    }


    if (rc != SQLITE_OK) {
        mvwprintw(child, 5, 2, "SQL Error: %s", errMsg);
        sqlite3_free(errMsg);

    } else if (row1 == 4) {
        mvwprintw(child, 4, 2, "Data Kosong / Tidak Ditemukan");
    }

}

void MenuBuy(WINDOW *win, LogSession *Curent, sqlite3 *db,int ch){
    WINDOW *child = derwin(win, 18, 58, 5, 1);
    
    werase(child);
    box (child, 0, 0);

    mvwprintw(child, 1 , 2, "%s", "LIST PRODUCT");
    mvwprintw(child, 3 , 2, "%s", "ID Sel");
    mvwprintw(child, 3 , 9, "%s", "ID Prod");
    mvwprintw(child, 3 , 19, "%s", "Name");
    mvwprintw(child, 3 , 41, "%s", "Qty");
    mvwprintw(child, 3 , 48, "%s", "Cost");

    AllListProduct(child, Curent, db, ch);

    wrefresh(child);
    if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->level = 0;
    }

    if (Curent->menu != MENU_SELL) {
        delwin(child);
    }
}
// END CODE =========================================================================