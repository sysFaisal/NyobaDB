
#include <ncurses/ncurses.h>
#include <string.h>
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
