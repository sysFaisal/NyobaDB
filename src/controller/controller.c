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
    int saldo;
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

    if (argv[2] == NULL) {
        temp->list[temp->count].saldo = 0;
    } else {
        temp->list[temp->count].saldo = atoi(argv[2]);
    }

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

    const char *promptsql = "SELECT Users.*, Wallet.saldo FROM Users LEFT JOIN Wallet ON Users.id_user = Wallet.id_user;"
            ;

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
        mvwprintw(child, 1 , 18, "%s", "Nama");
        mvwprintw(child, 1 , 29, "%s", "Saldo");

        wrefresh(child);

        int Select = KeypadsInput(c, &highlight, temp.count);

        if (Select != -1){
            choice = temp.list[highlight].id_user;  
        }

        mvwprintw(child, 16, 51, "%d : %d", highlight, highlight + 1);
        mvwprintw(child, 16, 2, ">  %d", temp.list[highlight].id_user);
        
        for (i = 0; i < temp.count; i++) {
            if (i == highlight){
                wattron(child, A_REVERSE);

                mvwprintw(child, row + i, 2, "%d", i + 1);
                mvwprintw(child, row + i, 7, "%d", temp.list[i].id_user);
                mvwprintw(child, row + i, 18, "%s", temp.list[i].nama);
                mvwprintw(child, row + i, 29, "%d", temp.list[i].saldo);

                wattroff(child, A_REVERSE);
            } else {

                mvwprintw(child, row + i, 2, "%d", i + 1);
                mvwprintw(child, row + i, 7, "%d", temp.list[i].id_user);
                mvwprintw(child, row + i, 18, "%s", temp.list[i].nama);
                mvwprintw(child, row + i, 29, "%d", temp.list[i].saldo);

            }
        }
    }
    
    delwin(child);
    return choice;
}

void LoginUser(WINDOW *win, LogSession *Curent, sqlite3 *db){
    int ChoiceId = PrintList(win, db);

    if (ChoiceId != -1) {
        ProsesLogin(db, ChoiceId, Curent);

        Curent->menu = MENU_MAIN;
        Curent->level = 0;

        Curent->highlight[0] = 0;

    }
}


void FixLogout(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch){
    (void)db;
    (void)ch;

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 2, 2, "Logging out...");
    wrefresh(win);

    napms(500);

    Curent->status = 0;
    Curent->menu   = MENU_MAIN;
    Curent->level  = 0;
    Curent->highlight[0] = 0;

    werase(win);

}

void MenuLogout(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch){
    (void)db;

    werase(win);
    box(win, 0, 0);

    mvwprintw(win, 2, 2, "LOG OUT");
    mvwprintw(win, 4, 2, "[ENTER] Logout");
    mvwprintw(win, 5, 2, "[B] Back");

    if (ch == '\n' || ch == KEY_ENTER) {
        FixLogout(win, Curent, db, ch);
    } else if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->level = 0;
    }

    wrefresh(win);
}

typedef struct {
    int flags;
    int saldo;
} WalletCheck;

int WalletCheckCallBack(void *dat, int argc, char **argv, char **azColName){
    (void)azColName;
    (void)argc;

    WalletCheck *temp1 = (WalletCheck *)dat;
    temp1->flags = 1;
    temp1->saldo = atoi(argv[0]);
    
    return 0;
}

void UpdateSaldo(WINDOW *child, LogSession *Curent, sqlite3 *db, int ch){
    (void)ch;
    WalletCheck temp1 = {0};

    char buff[256]; char buff2[20] = "";
    char *errMSG = NULL;

    snprintf(buff, sizeof(buff), 
        "SELECT saldo FROM Wallet WHERE id_user = %d;",
        Curent->id_user
    );

    echo();
    curs_set(1);

    mvwprintw(child, 16, 2, "Masukan Saldo : ");

    wmove(child, 16, 18);
    wgetnstr(child, buff2, sizeof(buff2) - 1);
    int Saldo = atoi(buff2);

    noecho();
    curs_set(0);

    werase(child);
    box(child, 0, 0);

    sqlite3_exec(db, buff, WalletCheckCallBack, &temp1, &errMSG);

    if (temp1.flags != 1) {
        snprintf(buff, sizeof(buff), 
        "INSERT INTO Wallet (id_user, saldo) VALUES (%d, %d);",
         Curent->id_user, Saldo
        );
    } else {
        snprintf(buff, sizeof(buff),
         "UPDATE Wallet SET saldo = %d WHERE id_user = %d;",
          Saldo, Curent->id_user
        );
    }

    int rc = sqlite3_exec(db, buff, NULL, NULL, &errMSG);

    if (rc != SQLITE_OK) {
            mvwprintw(child, 10, 2, "SQL Error: %s", errMSG);
            sqlite3_free(errMSG);
            wrefresh(child);
            wgetch(child);
            return;
        }

    mvwprintw(child, 10, 2, "Data berhasil disimpan!");
    mvwprintw(child, 12, 2, "Saldo baru : %d", Saldo);

    wrefresh(child);

    while ((ch = wgetch(child))) {
        if (ch == 'b' || ch == 'B' || ch == 10) {
            return;   
        }
    }
}


void UpdateNama(WINDOW *child,LogSession *Curent, sqlite3 *db, int ch){

    (void)ch;
    char buff[3000]; char buff2[50] = "";
    char *errMSG = NULL;

    echo();
    curs_set(1);

    mvwprintw(child, 16, 2, "Masukan Nama : ");

    wmove(child, 16, 17);
    wgetnstr(child, buff2, sizeof(buff2) - 1);

    noecho();
    curs_set(0);

    sprintf(buff, "UPDATE Users SET Nama = '%s' WHERE id_user = %d ;", buff2, Curent->id_user);
    int rc = sqlite3_exec(db, buff, NULL, NULL, &errMSG);

    if (rc != SQLITE_OK) {
            mvwprintw(child, 10, 2, "SQL Error: %s", errMSG);
            sqlite3_free(errMSG);
            wrefresh(child);
            wgetch(child);
            return;
        }   

    werase(child);
    box(child, 0, 0);
    mvwprintw(child, 10, 2, "Data berhasil disimpan!");
    wrefresh(child);

    while((ch = wgetch(child))){
        if (ch == 'b' || ch == 'B' || ch == 10){
            return;
        }
    }
    
}

typedef struct {
    char barang[100];
    int jumlah;
    int harga;
} SellProductView;

void SellProduct(WINDOW *child, LogSession *Curent, sqlite3 *db, int ch){
    (void)ch;

    SellProductView temp = {0};
    char buff[512];

    werase(child);
    box(child, 0, 0);

    mvwprintw(child, 2, 2, "=== SELL PRODUCT ===");
    mvwprintw(child, 4, 2, "Name   : ");
    mvwprintw(child, 5, 2, "Qty    : ");
    mvwprintw(child, 6, 2, "Cost   : ");
    mvwprintw(child, 8, 2, "[ENTER] Simpan");

    echo();
    curs_set(1);

    //Nama
    wmove(child, 4, 12);
    wgetnstr(child, buff, sizeof(buff) - 1);
    strncpy(temp.barang, buff, sizeof(temp.barang) - 1);

    //Qty
    wmove(child, 5, 12);
    wgetnstr(child, buff, 5);
    temp.jumlah = atoi(buff);

    //Harga
    wmove(child, 6, 12);
    wgetnstr(child, buff, 20);
    temp.harga = atoi(buff);

    noecho();
    curs_set(0);

    
    if (temp.harga <= 0 || temp.jumlah <= 0 || strlen(temp.barang) == 0) {
        mvwprintw(child, 10, 2, "Input tidak valid!");
        wrefresh(child);
        wgetch(child);
        return;
    }


    snprintf(
        buff,
        sizeof(buff),
        "INSERT INTO Product (id_user, Nama, Jumlah, Harga) "
        "VALUES (%d, '%s', %d, %d);",
        Curent->id_user,
        temp.barang,
        temp.jumlah,
        temp.harga
    );

    char *errMSG = NULL;
    int rc = sqlite3_exec(db, buff, NULL, NULL, &errMSG);

    if (rc != SQLITE_OK) {
        mvwprintw(child, 10, 2, "SQL Error: %s", errMSG);
        sqlite3_free(errMSG);
        wrefresh(child);
        wgetch(child);
        return;
    }

    mvwprintw(child, 10, 2, "Data berhasil disimpan!");
    wrefresh(child);

    while ((ch = wgetch(child))) {
        if (ch == 'b' || ch == 'B' || ch == 10) {
            return;   
        }
    }
}

void BuyProduct(WINDOW *child, LogSession *Curent, sqlite3 *db,
                int id_seller, int id_product, int jumlah,
                int harga, const char *nama)
{
    echo();
    curs_set(1);

    char qty[32];
    int jumlah_buy;

    mvwprintw(child, 16, 2, "Jumlah beli : ");
    wmove(child, 16, 16);
    wgetnstr(child, qty, sizeof(qty) - 1);

    noecho();
    curs_set(0);

    jumlah_buy = atoi(qty);


    if (jumlah_buy <= 0 || jumlah_buy > jumlah) {
        mvwprintw(child, 15, 2, "Jumlah beli tidak valid!");
        wrefresh(child);
        wgetch(child);
        return;
    }

    int totalharga = jumlah_buy * harga;
    if (totalharga > GetSaldo(Curent, db)) {
        mvwprintw(child, 15, 2, "Uang Tidak Cukup!");
        wrefresh(child);
        wgetch(child);
        return;
    }
    char sql[1024];
    char *errMSG = NULL;

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

    snprintf(sql, sizeof(sql) - 1,
        "INSERT INTO Transaksi "
        "(id_buyer, id_seller, id_product, jumlah, total_harga, nama_barang, harga) "
        "VALUES (%d, %d, %d, %d, %d, '%s', %d);",
        Curent->id_user, id_seller, id_product,
        jumlah_buy, totalharga, nama, harga
    );

    if (sqlite3_exec(db, sql, NULL, NULL, &errMSG) != SQLITE_OK)
        goto rollback;

    snprintf(sql, sizeof(sql),
        "UPDATE Product SET Jumlah = Jumlah - %d WHERE id_product = %d;",
        jumlah_buy, id_product
    );
    if (sqlite3_exec(db, sql, NULL, NULL, &errMSG) != SQLITE_OK)
        goto rollback;

    snprintf(sql, sizeof(sql),
        "UPDATE Wallet SET saldo = saldo + %d WHERE id_user = %d;",
        totalharga, id_seller
    );
    sqlite3_exec(db, sql, NULL, NULL, NULL);

    snprintf(sql, sizeof(sql),
        "UPDATE Wallet SET saldo = saldo - %d WHERE id_user = %d;",
        totalharga, Curent->id_user
    );
    sqlite3_exec(db, sql, NULL, NULL, NULL);

    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

    mvwprintw(child, 18, 2, "Transaksi berhasil!");
    wrefresh(child);
    wgetch(child);
    return;

rollback:
    sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
    mvwprintw(child, 18, 2, "Transaksi gagal: %s", errMSG);
    sqlite3_free(errMSG);
    wrefresh(child);
    wgetch(child);
}
