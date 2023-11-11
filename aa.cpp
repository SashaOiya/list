#include <stdio.h>

struct List_t {    //  0   1   2   3   4   5   6   7   8   9  10  11  12
    int code[13] = { 111, 10, 20, 30, 40, 50, 60, 70, 80,  0,  0,  0,  0 };
    int next[13] = { 0,    2,  3,  4,  5,  6,  7,  8,  0, 10, 11, 12, 13 };
    int prev[13] = { 0,    0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1 };
    int free = 9;
};

void Dump ( struct List_t list );
void List_Insert ( struct List_t *list, int prev, int code );
void List_Delete ( struct List_t *list, int place );

int main ()
{
    struct List_t list;
    List_Delete ( &list, 3 );
    List_Insert ( &list, 4, 45 );
    List_Insert ( &list, 7, 75 );
    List_Delete ( &list, 4 );
    List_Insert ( &list, 7, 74 );

    return 0;
}

void Dump ( struct List_t list )
{
    printf ( "       \t" );
    for ( int i = 0; i < 13; ++i ) {
        printf ( "%3d\t", i );
    }

    printf ( "\ncode : \t" );
    for ( int i = 0; i < 13; ++i ) {
        printf ( "%3d\t", list.code[i] );
    }

    printf ( "\nnext : \t" );
    for ( int i = 0; i < 13; ++i ) {
        printf ( "%3d\t", list.next[i] );
    }

    printf ( "\nprev : \t" );
    for ( int i = 0; i < 13; ++i ) {
        printf ( "%3d\t", list.prev[i] );
    }

    int temp = list.next[1];
    printf ( "\nNcod :      \t" );
    printf ( "%3d\t", list.code[list.prev[temp]] );
    for ( int i = 0; i < 13; ++i ) {
        printf ( "%3d\t", list.code[temp] );
        if ( list.next[temp] == 0 ) {
            printf ( "\n" );
            break;
        }
        temp = list.next[temp];
    }
    printf ( "free : %d\n\n", list.free );
}
                                           // name
void List_Insert ( struct List_t *list, int place, int code )
{
    int new_place = 1;
    for ( int i = 0; i < place - 1; ++i ) {
        new_place = list->next[new_place];
    }

    int prev_free = list->free;
    int temp_next = list->next[new_place];  //
    int temp_free = list->next[list->free];

    list->free = list->next[prev_free];
    list->next[new_place] = prev_free;
    list->next[prev_free] = temp_next;
    //list->next[prev] = next;
    //list->next[next] = temp;
    list->code[prev_free] = code;
    int temp_prev = list->prev[new_place+1];
    list->prev[new_place+1] = prev_free;
    list->prev[prev_free] = temp_prev;
    Dump ( *list );
}

void List_Delete ( struct List_t *list, int place )
{
    int new_place = 1;
    for ( int i = 0; i < place - 1; ++i ) {
        new_place = list->next[new_place];
    }

    int temp_prev = list->prev[new_place];
    int temp_next = list->next[new_place];
    list->next[place] = list->free;
    //list->next[list->free] = place;
    list->free = new_place;
    list->prev[place] = -1;
    list->next[temp_prev] = temp_next;
    list->prev[temp_next] = temp_prev;
    Dump ( *list );
}

