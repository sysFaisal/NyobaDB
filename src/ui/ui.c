
#include <ncurses/ncurses.h>
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

void MenuMain(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch) {
    wrefresh(win);

    WINDOW *child = derwin(win, 18, 58, 5, 1);

    if (Curent->status == 0) {
        LoginUser(win, Curent, db);
        
        return; 
    }

    wrefresh(win);
    box(win, 0, 0);
    box(child, 0, 0);

    int ChoiceMenu = -1;

    if (ch != -1) {
        ChoiceMenu = KeypadsInput(ch, &Curent->highlight, 5);
    }

    const char *Menu[5] = {"1. Profile", "2. Sell", "3. Buy", "4. Chat", "5. Logout"};
    mvwprintw(child, 16, 2, "[DEBUG] Curent > %d", Curent->highlight);

    for (int i = 0; i < 5; i++) {
        if (i == Curent->highlight) {
            wattron(child, A_REVERSE);
            mvwprintw(child, i + 1, 2, " > %s ", Menu[i]); 
            wattroff(child, A_REVERSE);
            mvwprintw(child, 15, 2, "Pilihan > %s", Menu[Curent->highlight]);
        } else {
            mvwprintw(child, i + 1, 2, "   %s ", Menu[i]); 
        }
    }

    if (ChoiceMenu == 1) {
        switch (Curent->highlight) {
            case 0: Curent->menu = MENU_PROFILE; break;
            case 1: Curent->menu = MENU_SELL; break;
            case 2: Curent->menu = MENU_BUY; break;
            case 3: Curent->menu = MENU_CHAT; break;
            case 4: Curent->menu = MENU_LOGOUT; break;
        }
        Curent->highlight = 0;
    }
    delwin(child);
    wrefresh(win);
}

// PROFILE MENU =========================================================================
void ProfilePilihan(WINDOW *child, LogSession *Curent, sqlite3 *db,int ch){
    int i;
    const char *Pilihan[2] = {"1. Update Saldo", "2. Update Nama"};

    int Status = KeypadsInput(ch, &Curent->highlight_profile, 2);

    if (Status == 1) {
        switch (Curent->highlight_profile) {
            case 0: mvwprintw(child, 15, 2, "Pilihan > %s", Pilihan[Curent->highlight_profile]); break;
            case 1: mvwprintw(child, 15, 2, "Pilihan > %s", Pilihan[Curent->highlight_profile]); break;
        }
    }

    int row = 7;
    for (i = 0; i < 2; i++){
        if (i == Curent->highlight_profile){
            wattron(child, A_REVERSE);
            mvwprintw(child, row + i, 2, "> %s", Pilihan[i]);
            wattroff(child, A_REVERSE);
        } else {
            mvwprintw(child, row + i, 2, "> %s", Pilihan[i]);
        }
    }

    mvwprintw(child, 16, 2, "[DEBUG] Curent %d", Curent->highlight_profile);

    if (Curent->highlight_profile >= 0 && Curent->highlight_profile < 2) {
        mvwprintw(child, 15, 2, "Choice > %s", Pilihan[Curent->highlight_profile]);

    }

    if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->highlight = 0;
    }
}


void MenuProfile(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch){

    WINDOW *child = derwin(win, 18, 58, 5, 1);
    WINDOW *foto = derwin(child, 5, 9, 1, 2);

    box(child, 0, 0);
    box(foto, 0, 0);

    ProfilePilihan(child, Curent, db, ch);

    mvwprintw(child, 2, 12, "ID User : %d", Curent->id_user);
    mvwprintw(child, 3, 12, "Nama : %s", Curent->nama);
    mvwprintw(child, 4, 12, "Saldo : ");
    mvwprintw(child, 5, 12, "Product : ");

    if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->highlight = 0;
        delwin(child);
        delwin(foto);
    }
}
// END CODE =========================================================================

