
#include <ncurses/ncurses.h>
#include <string.h>
#include "controller/controller.h"
#include "ui/ui.h"
#include "struct.h"

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

static void draw_left(){
    box(left, 0, 0);
}
static void draw_right(){
    box(right, 0, 0);
}

void ui_draw(){
    draw_top();
    draw_left();
    draw_right();
}

void MenuMain(WINDOW *win, LogSession *Curent, sqlite3 *db, int ch) {
    
    if (Curent->status == 0) {
        LoginUser(win, Curent, db);
        
        return; 
    }
    wrefresh(win);
    box(win, 0, 0);

    int ChoiceMenu = -1;

    if (ch != -1) {
        ChoiceMenu = KeypadsInput(ch, &Curent->highlight, 5);
    }

    box(win, 0, 0);
    mvwprintw(win, 2, 2, "ID   : %d", Curent->id_user);
    mvwprintw(win, 3, 2, "Nama   : %s", Curent->nama);

    const char *Menu[5] = {"1. Profile", "2. Sell", "3. Buy", "4. Chat", "5. Logout"};
    
    for (int i = 0; i < 5; i++) {
        if (i == Curent->highlight) {
            wattron(win, A_REVERSE);
            mvwprintw(win, i + 6, 2, " > %s ", Menu[i]); 
            wattroff(win, A_REVERSE);
            mvwprintw(win, 19, 2, "Pilihan > %s", Menu[Curent->highlight]);
        } else {
            mvwprintw(win, i + 6, 2, "   %s ", Menu[i]); 
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
}

void MenuProfile(WINDOW *win, LogSession *Curent, int ch)
{
    box(win, 0, 0);
    mvwprintw(win, 2, 2, "=== PROFILE ===");
    mvwprintw(win, 4, 2, "ID   : %d", Curent->id_user);
    mvwprintw(win, 5, 2, "Nama : %s", Curent->nama);
    mvwprintw(win, 7, 2, "[B] Back");

    if (ch == 'b' || ch == 'B') {
        Curent->menu = MENU_MAIN;
        Curent->highlight = 0;
    }
}


