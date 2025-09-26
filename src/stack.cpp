#include "stack.h"

ErrStack_t StackCtor( Stack_t* stk, size_t capacity ) {
    stk->ptr = ( int* ) calloc ( capacity, sizeof( int ) );
    stk->capacity = capacity;

    return ERR_NONE;
}

ErrStack_t StackDtor( Stack_t* stk ) {
    free( stk->ptr );

    return ERR_NONE;
}

ErrStack_t StackPush( Stack_t* stk, int element ) {
    if ( stk->size + 1 > stk->capacity ) {
        stk->capacity *= 2;
        int* new_ptr = ( int* ) realloc ( stk->ptr, stk->capacity * sizeof( int ) );
        assert( stk->ptr != NULL );

        stk->ptr = new_ptr;
    }

    *( stk->ptr + stk->size ) = element;
    stk->size++;

    return ERR_NONE;
}

ErrStack_t StackPop( Stack_t* stk ) {
    stk->size--;
    *( stk->ptr + stk->size ) = 0;

    if ( stk->size * 4 <= stk->capacity ) {
        stk->capacity /= 2;
        stk->ptr = ( int* ) realloc ( stk->ptr, stk->capacity * sizeof( int ) );
    }

    return ERR_NONE;
}

ErrStack_t StackVerify( Stack_t* stk ) {
    if ( stk->ptr != NULL ) {
        stk->varInfo.err_code += ERR_BADPTR;
    }
    if ( stk->capacity < stk->size ) {
        stk->varInfo.err_code += ERR_SIZEOVERCAPACITY;
    }

    return ERR_NONE;
}

ErrStack_t StackDump( Stack_t* stk ) {
    assert( stk != NULL );
    fprintf( stderr, "stack<int>[%p] \"%s\" FILE: %s --- LINE: %lu --- FUNC: %s\n"
                     // "   called at %s::%lu \n"
                     "   {\n"
                     "   capacity = %lu\n"
                     "   size     = %lu\n"
                     "   data[%lu] = [%p]\n",
            stk, stk->varInfo.name, stk->varInfo.file, stk->varInfo.line, stk->varInfo.func,
            stk->capacity,
            stk->size,
            stk->size, stk->ptr );

    printf( TWO_TAB "{\n");
    for ( size_t i = 0; i < stk->capacity; i++ ) {
        if ( i < stk->size ) {
            fprintf( stderr, TWO_TAB "*[%lu] = %d\n", i, *( stk->ptr + i ) );
        }
        else {
            fprintf( stderr, TWO_TAB " [%lu] = %d ( poison )\n", i, *( stk->ptr + i ) );
        }
    }
    printf( TWO_TAB "}\n"
            "   }\n" );

    return ERR_NONE;
}



/* Использовал до появления StackDump */
// void StackPrint( Stack_t stk ) {
//     StackStat( stk );

//     for ( size_t i = 0; i < stk.size; i++ ) {
//         fprintf(  stderr, "           stk[%lu] = %d\n", i, *( stk.ptr + i ) );
//     }

//     printf( "\n\n" );
// }

// void StackStat( Stack_t stk ) {
//     printf( "StackStat: ptr      - %p\n"
//             "           size     - %lu\n"
//             "           capacity - %lu\n\n",
//             stk.ptr, stk.size, stk.capacity );
// }
