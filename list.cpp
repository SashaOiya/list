#include "list.h"

void Text_Dump ( struct List_t *list )
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

void List_Insert ( struct List_t *list, int place, int value )
{
    ++list->size;
    if ( list->capacity <= list->size ) {
        List_Realloc ( list );
    }

    const int prev_free   = list->free;
    const int temp_next   = list->next[place];
    const int temp_free   = list->next[list->free];

    list->free            = list->next[prev_free];
    list->next[place] = prev_free;
    list->next[prev_free] = temp_next;
    list->code[prev_free] = value;

    if ( prev_free > list->tail ) {
        list->tail = prev_free;
    }
    list->prev[list->next[prev_free]] = prev_free;
    list->prev[prev_free]             = place;

    Text_Dump ( list );
}

void List_Delete ( struct List_t *list, int place )
{
    if ( place == list->tail ) {
        list->tail = list->prev[list->tail];
    }
    if ( place == list->head ) {
        list->next[0] = list->next[place];
    }
    --(list->size);

    list->code[place] = 0;

    int temp_prev     = list->prev[place];
    int temp_next     = list->next[place];
    list->next[place] = list->free;
    list->free        = place;

    list->prev[place] = -1;

    list->next[temp_prev] = temp_next;
    list->prev[temp_next] = temp_prev;

    Text_Dump ( list );
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
    list->size     = buffer_size;
    list->capacity = list->size;

    list->code     = ( elem_t *)calloc ( list->capacity, sizeof (elem_t) );
    list->next     = ( elem_t *)calloc ( list->capacity, sizeof (elem_t) );
    list->prev     = ( elem_t *)calloc ( list->capacity, sizeof (elem_t) );

    assert ( list->code != nullptr );   //if

    memcpy ( list->code + 1, start_buffer, ( list->size ) * sizeof ( elem_t ) );
    list->code[0] = POISON;

    bool free_flag = false;

    for ( int i = list->head; i < list->capacity; ++i ) {
            list->next[i] = i + 1;
            list->prev[i] = i - 1;
    }
    list->next[list->capacity-1] = 0;
    list->free                   = list->capacity;
    list->tail                   = list->capacity - 1;
    list->next[0]                = list->head;

    Text_Dump ( list );
}

void List_Realloc ( struct List_t *list )
{
    assert ( list       != nullptr );
    assert ( list->code != nullptr );

    list->capacity *= 2;

    list->code = ( elem_t *) realloc ( list->code, list->capacity * sizeof ( elem_t ) );
    list->next = ( int *)    realloc ( list->next, list->capacity * sizeof ( int )    );
    list->prev = ( int *)    realloc ( list->prev, list->capacity * sizeof ( int )    );

    assert ( list->code != nullptr );
    assert ( list->next != nullptr );
    assert ( list->prev != nullptr );

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

    //system ( "del list.png" );
    system ( "dot -T png list.dot -o list.png" );
    system ( "list.png" );

    fclose ( dot );
}

void List_Dtor ( struct List_t *list )
{
    assert ( list != nullptr );

    free ( list->code );
    free ( list->next );
    free ( list->prev );

    list->code = list->next = list->prev = nullptr;

}


