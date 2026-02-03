#ifndef STRUCT_H
#define STRUCT_H

#define MAX_NAME 100

typedef enum {
    MENU_MAIN,
    MENU_PROFILE,
    MENU_SELL,
    MENU_BUY,
    MENU_CHAT,
    MENU_LOGOUT
} MenuState;

typedef struct {
    int id_user;
    char nama[MAX_NAME];
    int status;
    int highlight;
    MenuState menu;
} LogSession;


#endif