#ifndef LIST
#define LIST

#include <stdio.h>            // ERROR +
#include <stdlib.h>
#include <assert.h>
#include <cstring>

const int POISON = 5674;

typedef int elem_t;

struct List_t {
    elem_t *code = 0;
    int    *next = 0;
    int    *prev = 0;
    int     free = 0;
    int     size = 0;
    int capacity = 0;
    int     tail = 1;
    int     head = 1;
};

void List_Ctor    ( struct List_t *list, elem_t start_buffer[], int buffer_size );
void Text_Dump    ( const struct List_t *list );
void List_Realloc ( struct List_t *list );
void List_Insert  ( struct List_t *list, int prev, int value );
void List_Delete  ( struct List_t *list, int index );
int  List_Search  ( struct List_t *list, int search_location );
void Graph_Dump   ( const struct List_t *list );
void List_Dtor    ( struct List_t *list );   // ERROR +

#endif      // LIST
