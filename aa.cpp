#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>

const int POISON = 5674;

typedef int elem_t;

struct List_t {
    elem_t *code = 0;;
    int *next = 0;
    int *prev = 0;
    int free = 0;
    int size = 0;
    int capacity = 0;
    int tail = 1;
    int head = 1;
};

void List_Ctor   ( struct List_t *list, elem_t start_buffer[], int buffer_size );
void Static_Dump ( const struct List_t *list );
void List_Realloc( struct List_t *list );
void List_Insert ( struct List_t *list, int prev, int value );
void List_Delete ( struct List_t *list, int index );
int  List_Search ( struct List_t *list, int search_location );
void Graph_Dump  ( const struct List_t *list );

int main ()
{
    struct List_t list = {};

    elem_t code[] = { 8, 8, 8, 8, 8, 0 };

    List_Ctor ( &list, code, sizeof ( code ) / sizeof ( elem_t ) ); // size
    //++list.data_size;
    //List_Realloc ( &list );

    //List_Insert ( &list, 8, 45 );
    //List_Insert ( &list, 5, 76 );
    //List_Delete ( &list, 3 );
    List_Insert ( &list, 2, 10 );
    List_Insert ( &list, 2, 122 );
    //List_Insert ( &list, 2, 66 );
    //List_Insert ( &list, 7, 75 );
    //List_Delete ( &list, 4 );
    //List_Insert ( &list, 7, 74 );

    return 0;
}

void Static_Dump ( struct List_t *list )
{
    printf ( "       \t" );
    for ( int i = 0; i < list->capacity; ++i ) {
        printf ( "%3d\t", i );
    }

    printf ( "\ncode : \t" );
    for ( int i = 0; i < list->capacity; ++i ) {
        printf ( "%3d\t", list->code[i] );
    }

    printf ( "\nnext : \t" );
    for ( int i = 0; i < list->capacity; ++i ) {
        printf ( "%3d\t", list->next[i] );
    }

    printf ( "\nprev : \t" );
    for ( int i = 0; i < list->capacity; ++i ) {
        printf ( "%3d\t", list->prev[i] );
    }

    int temp = list->next[list->head];
    printf ( "\nNcod :      \t" );
    printf ( "%3d\t", list->code[list->prev[temp]] );
    for ( int i = list->head; i < list->size - 1; ++i ) {
        if ( list->code[temp] != 0 ) {
            printf ( "%3d\t", list->code[temp] );
        }
        if ( i == list->tail ) {
            break;
        }
        temp = list->next[temp];
    }
    printf ( "\nfree : %d\n", list->free );
    printf ( "head : %d\ntail : %d\n\n", list->head, list->tail );
}

void List_Insert ( struct List_t *list, int index, int value )
{
    ++list->size;
    if ( list->capacity <= list->size ) {
        List_Realloc ( list );
    }

    int new_place = 1;
    for ( int i = 0; i < index - 1; ++i ) {
        new_place = list->next[new_place];
    }

    const int prev_free = list->free;
    const int temp_next = list->next[new_place];
    const int temp_free = list->next[list->free];

    list->free = list->next[prev_free];
    list->next[new_place] = prev_free;
    list->next[prev_free] = temp_next;
    list->code[prev_free] = value;

    if ( prev_free > list->tail ) {
        list->tail = prev_free;
    }
    list->prev[list->next[prev_free]] = prev_free;
    list->prev[prev_free] = new_place;

    Static_Dump ( list );
}

void List_Delete ( struct List_t *list, int place )
{
    int new_place = 1;     // delete free
    for ( int i = 0; i < place - 1; ++i ) {
        new_place = list->next[new_place];
    }
    if ( new_place == list->tail ) {
        list->tail = list->prev[list->tail];
    }
    --(list->size);
    list->code[new_place] = 0;
    int temp_prev = list->prev[new_place];
    int temp_next = list->next[new_place];
    list->next[new_place] = list->free;
    list->free = new_place;
    list->prev[new_place] = -1;
    list->next[temp_prev] = temp_next;
    list->prev[temp_next] = temp_prev;

    Static_Dump ( list );
}

int List_Search ( struct List_t *list, int search_location )
{
    int new_place = 1;
    for ( int i = 0; i < search_location - 1; ++i ) {
        new_place = list->next[new_place];
    }

    return new_place;
}

void List_Ctor   ( struct List_t *list, elem_t start_buffer[], int buffer_size )
{
    list->size = buffer_size;
    list->capacity = list->size + 1;

    list->code = ( elem_t *)calloc ( list->capacity, sizeof (elem_t) );
    list->next = ( elem_t *)calloc ( list->capacity, sizeof (elem_t) );
    list->prev = ( elem_t *)calloc ( list->capacity, sizeof (elem_t) );

    assert ( list->code != nullptr );   //if

    memcpy ( list->code + 1, start_buffer, ( list->size ) * sizeof ( elem_t ) );
    list->code[0] = POISON;

    bool free_flag = false;
                               // function
    for ( int i = list->head; i < list->capacity; ++i ) {     //code[1]!=0
        if ( list->code[i+1] != 0  ) {
            int prev_tail = list->tail;
            list->next[i] = i + 1;
            list->tail = ( i < list->capacity - 1 ) ? i + 1 : list->tail ;
        }
        else if ( list->code[i+1] == 0 ) {
            if ( free_flag == false ) {
                list->free = i + 1;
                free_flag = true;
            }
            list->next[i] = i + 1;
            list->next[list->tail] = 0;
        }
        if ( list->code[i - 1] != 0 ) {
            list->prev[i] = i - 1;
        }
        else {
            list->prev[i] = -1;
        }
    }

    Static_Dump ( list );
}

void List_Realloc ( struct List_t *list )
{
    assert ( list != nullptr );

    list->capacity *= 2;

    list->code = ( elem_t *)realloc ( list->code, list->capacity * sizeof ( elem_t ) );
    list->next = ( int *)realloc ( list->next, list->capacity * sizeof ( int ) );
    list->prev = ( int *)realloc ( list->prev, list->capacity * sizeof ( int ) );

    assert ( list->code != nullptr );
    assert ( list->next != nullptr );
    assert ( list->prev != nullptr );
                               // function
    for ( int i = list->capacity / 2; i < list->capacity; ++i ) {
        list->code[i] = 0;
        list->next[i] = i + 1;
        list->prev[i] = -1;
    }
}

void Graph_Dump ( const struct List_t *list )
{
    FILE *dot = fopen ( "list.dot", "w" );

    fprintf ( dot, "digraph G { \n"
                   "rankdir = LR;\n"
                   "node [shape = record];\n"
                   " 0 " );
    for ( int i = 0; i < list->size; ++i ) {
        fprintf ( dot, "-> %d", i );
    }
    fprintf ( dot, "[arrowsize = 0.0, weight = 10000, color = \"#FFFFFF\"];\n" );
                  // " list [shape = Mrecord, style = filled, fillcolor = lightpink, "
                   //" label = \" data: %d | free: %d \"];)\n", list.data, list.free );
    for ( int i = 0; i < list->size; ++i ) {
        fprintf ( dot, "%d [shape = Mrecord, style = filled, fillcolor = lightpink "
                       " label = \"idx: %d | data: %d | next: %d | prev: %d\"];\n ",i, i, list->code[i],
                                                                                          list->next[i],
                                                                                          list->prev[i] );
    }
    for ( int i = 0; i < list->size; ++i ) {
        fprintf ( dot, "%d -> %d;\n", i, list->next[i] );
    }
    fprintf ( dot, "}\n" );

    system ( "dot -T png list.dot -o list.png" );
    system ( "list.png" );

    fclose ( dot );
}

//void List_Dtor ( const struct List_t *list )


