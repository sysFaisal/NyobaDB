#ifndef STRUCT_H
#define STRUCT_H

#define MAX_NAME 100

typedef struct {
    int id_user;
    char nama[MAX_NAME];
    int status;
} LogSession;

typedef struct{
    int id_user;
    char nama[MAX_NAME];
} UserList;

typedef struct {
    UserList *List;
    int row;
} UserData;

#endif