#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "c_db/sqlite3.h"
#include "curses.h"

#define DB_NAME "db/data1.db"
#define MAX_NAME 100

typedef struct {
    int id_user;
    char nama[MAX_NAME];
} LogSession;

typedef struct {
    WINDOW *win;
    int row;
} WinTable;

/* ================= UTIL ================= */

int desimal(const char *s) {
    int hasil = 0;
    for (int i = 0; s[i]; i++)
        if (s[i] >= '0' && s[i] <= '9')
            hasil = hasil * 10 + (s[i] - '0');
    return hasil;
}

bool CekFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return false;
    fclose(fp);
    return true;
}

/* ================= SQLITE CALLBACK ================= */

int PrintUserCallBack(void *data, int argc, char **argv, char **col) {
    WinTable *tbl = (WinTable *)data;
    WINDOW *win = tbl->win;

    if (tbl->row == 0) {
        mvwprintw(win, 1, 2, "No ");
        int x = 6;
        for (int i = 0; i < argc; i++) {
            mvwprintw(win, 1, x, "| %-10s ", col[i]);
            x += 13;
        }
        mvwprintw(win, 2, 2,
            "------------------------------------------------------------");
    }

    tbl->row++;
    mvwprintw(win, tbl->row + 3, 2, "%-2d ", tbl->row);

    int x = 6;
    for (int i = 0; i < argc; i++) {
        mvwprintw(win, tbl->row + 3, x, "| %-10s ",
                  argv[i] ? argv[i] : "NULL");
        x += 13;
    }

    wrefresh(win);
    return 0;
}

int LoginCallBack(void *data, int argc, char **argv, char **col) {
    (void)argc; (void)col;
    LogSession *s = (LogSession *)data;

    s->id_user = argv[0] ? desimal(argv[0]) : -1;
    strcpy(s->nama, argv[1] ? argv[1] : "Tidak Ada");
    return 0;
}

/* ================= DATABASE ================= */

bool PrintAllUser(sqlite3 *db, WINDOW *win) {
    char *err = NULL;
    WinTable tbl = { win, 0 };

    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Daftar User ");

    sqlite3_exec(db, "SELECT id_user, nama FROM Users;",
                 PrintUserCallBack, &tbl, &err);

    if (err) {
        sqlite3_free(err);
        return false;
    }

    wrefresh(win);
    return tbl.row > 0;
}

bool ProsesLogin(sqlite3 *db, int id, LogSession *out) {
    char sql[128];
    char *err = NULL;

    sprintf(sql,
        "SELECT id_user, nama FROM Users WHERE id_user=%d;", id);

    out->id_user = -1;
    strcpy(out->nama, "");

    sqlite3_exec(db, sql, LoginCallBack, out, &err);

    if (err) sqlite3_free(err);
    return out->id_user != -1;
}

/* ================= MAIN ================= */

int main() {
    sqlite3 *db;
    LogSession session = {0};

    /* ===== CEK DATABASE ===== */
    if (!CekFile(DB_NAME)) {
        printf("Database tidak ditemukan: %s\n", DB_NAME);
        return 1;
    }

    if (sqlite3_open(DB_NAME, &db) != SQLITE_OK) {
        printf("Gagal membuka database\n");
        return 1;
    }

    /* ===== INIT PDCURSES ===== */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
    refresh();
    
    /* ===== WINDOW LIST USER ===== */
    // Window ini tetap di posisi atas
    WINDOW *listWin = newwin(15, 70, 1, 2);
    box(listWin, 0, 0);
    mvwprintw(listWin, 0, 2, " Daftar User ");
    
    PrintAllUser(db, listWin);
    wnoutrefresh(listWin);
    doupdate();

    /* ===== WINDOW INPUT ===== */
    // Window input di bawah list
    WINDOW *inputWin = newwin(7, 70, 17, 2);
    box(inputWin, 0, 0);
    mvwprintw(inputWin, 1, 2, "Masukkan ID User : ");
    keypad(inputWin, TRUE);

    char input[10] = {0};
    int idx = 0;
    int ch;

    wmove(inputWin, 1, 22);
    wnoutrefresh(inputWin);
    doupdate();

    /* ===== INPUT LOOP ===== */
    while (1) {
        ch = wgetch(inputWin);

        if (ch == '\n' || ch == KEY_ENTER)
            break;

        if ((ch == KEY_BACKSPACE || ch == 127 || ch == 8) && idx > 0) {
            idx--;
            input[idx] = '\0';
            mvwaddch(inputWin, 1, 22 + idx, ' ');
            wmove(inputWin, 1, 22 + idx);
        }
        else if (ch >= '0' && ch <= '9' && idx < 9) {
            input[idx++] = ch;
            mvwaddch(inputWin, 1, 22 + idx - 1, ch);
        }

        mvwprintw(inputWin, 3, 2, "[DEBUG] input = %-9s", input);
        wrefresh(inputWin); // Gunakan wrefresh untuk update instan saat ngetik
    }

    /* ===== PROSES LOGIN ===== */
    int targetID = desimal(input);
    bool sukses = ProsesLogin(db, targetID, &session);

    /* ===== WINDOW RESULT (PERBAIKAN) ===== */
    // UBAH DISINI: Jangan di 26, taruh di tengah (overlap) agar muat di layar
    // y=6, x=10, tinggi=10, lebar=54
    WINDOW *resultWin = newwin(10, 54, 6, 10); 

    // SAFETY CHECK: Jika terminal kekecilan, newwin return NULL
    if (resultWin == NULL) {
        endwin();
        printf("ERROR: Ukuran terminal terlalu kecil untuk menampilkan hasil.\n");
        printf("Mohon perbesar jendela terminal anda.\n");
        sqlite3_close(db);
        return 1;
    }

    // Buat window result menonjol (Clear area background window popup)
    wbkgd(resultWin, A_NORMAL); 
    werase(resultWin);
    box(resultWin, 0, 0);
    mvwprintw(resultWin, 0, 2, " Hasil Login ");

    mvwprintw(resultWin, 2, 2, "Status ID: %d", targetID);

    if (sukses) {
        // Tampilkan Sukses
        wattron(resultWin, A_BOLD);
        mvwprintw(resultWin, 4, 2, "[ SUKSES ]");
        wattroff(resultWin, A_BOLD);
        
        mvwprintw(resultWin, 5, 2, "User ID : %d", session.id_user);
        mvwprintw(resultWin, 6, 2, "Nama    : %s", session.nama);
    } else {
        // Tampilkan Gagal
        wattron(resultWin, A_BOLD);
        mvwprintw(resultWin, 4, 2, "[ GAGAL ]");
        wattroff(resultWin, A_BOLD);
        
        mvwprintw(resultWin, 5, 2, "User dengan ID tersebut");
        mvwprintw(resultWin, 6, 2, "tidak ditemukan.");
    }

    mvwprintw(resultWin, 8, 2, "Tekan Sembarang Tombol...");
    
    // Pastikan refresh dilakukan agar popup muncul di atas window lain
    wrefresh(resultWin); 

    /* ===== TAHAN PROGRAM ===== */
    // Program akan diam disini menunggu user menekan tombol
    wgetch(resultWin); 

    /* ===== CLEAN UP ===== */
    delwin(listWin);
    delwin(inputWin);
    delwin(resultWin);
    endwin();

    /* ===== DEBUG OUTPUT FINAL ===== */
    printf("DEBUG: Program Selesai Normal.\n");
    printf("Logged In: %s (ID: %d)\n", session.nama, session.id_user);
    
    sqlite3_close(db);
    return 0;
}

