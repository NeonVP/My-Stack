#include "stack.h"

void StackCtor( Stack_t* stk, size_t capacity ) {
    stk->ptr      = ( int* ) calloc ( capacity ON_DEBUG( + 2 ), sizeof( int ) );        // canaries
    assert( stk->ptr != NULL );

    stk->capacity = capacity;
    stk->size     = 0;

    #ifdef _DEBUG               // canaries
           stk->canaries_ptr[0]   = stk->ptr;
        *( stk->canaries_ptr[0] ) = canary;

           stk->canaries_ptr[1]   = stk->ptr + capacity + 1;
        *( stk->canaries_ptr[1] ) = canary;

        stk->ptr++;
    #endif

    for ( size_t i = 0; i < capacity; i++ ) {
        *( stk->ptr + i ) = poison;
    }

    ON_DEBUG( StackVerify( stk ); )
}

void StackDtor( Stack_t* stk ) {            // TODO: change StackDtor
    ON_DEBUG( stk->ptr--; )
    free( stk->ptr );
    // free( stk );
}

void StackPush( Stack_t* stk, int element ) {
    assert( stk != NULL );

    ON_DEBUG(
        if ( StackVerify( stk ) != ERR_NONE ) {
            return;
        }
    )

    if ( stk->size + 1 > stk->capacity ) {
        size_t new_capacity = stk->capacity * 2;

        StackRealloc( stk, new_capacity );
    }

    *( stk->ptr + stk->size ) = element;
    stk->size++;

    ON_DEBUG( StackVerify( stk ); )
}

int StackPop( Stack_t* stk ) {
    ON_DEBUG(
        if ( StackVerify( stk ) != ERR_NONE ) {
            return 0;
        }
    )

    stk->size--;
    int value = *( stk->ptr + stk->size );
    *( stk->ptr + stk->size ) = poison;

    if ( stk->size * 4 <= stk->capacity ) {
        // stk->capacity /= 2;
        // stk->ptr = ( int* ) realloc ( stk->ptr, stk->capacity * sizeof( int ) );
        // assert( stk->ptr != NULL );
        StackRealloc( stk, stk->capacity / 2 );
    }

    ON_DEBUG(
        if ( StackVerify( stk ) != ERR_NONE ) {
            return 0;
        }
    )

    return value;
}

#ifdef _DEBUG
long StackVerify( Stack_t* stk ) {
    if ( stk == NULL ) {
        stk->varInfo.err_code += ERR_BAD_PTR_STRUCT;
    }

    if ( stk->ptr == NULL ) {
        stk->varInfo.err_code += ERR_BAD_PTR_DATA;
    }

    if ( *( stk->canaries_ptr[0] ) != canary || *( stk->canaries_ptr[1] ) != canary ) {
        stk->varInfo.err_code += ERR_CORRUPTED_CANARY;
    }

    if ( stk->capacity < stk->size ) {
        stk->varInfo.err_code += ERR_SIZE_OVER_CAPACITY;
    }
    else {
        for ( size_t i = 0; i < stk->size; i++ ) {
            if ( *( stk->ptr + i ) == poison ) {
                stk->varInfo.err_code += ERR_POISON_IN_FILLED_CELLS;
                break;
            }
        }
    }

    if ( stk->capacity > 1000000 ) {
        stk->varInfo.err_code += ERR_LARGE_CAPACITY;
    }

    if ( stk->capacity == 0 ) {
        stk->varInfo.err_code += ERR_CORRUPTED_CAPACITY;
    }

    if ( stk->varInfo.err_code != ERR_NONE ) {
        StackDump( stk );
    }

    return stk->varInfo.err_code;
}

void ErrorProcessing( long err_code ) {             // TODO: add more errors
    if ( err_code & ERR_BAD_PTR_STRUCT ) {
        fprintf( stderr, "\tERROR: INVALID pointer to the structure with parameters  \n" );
        return;
    }

    if ( err_code & ERR_BAD_PTR_DATA ) {
        fprintf( stderr, "\tERROR: INVALID pointer to the stack \n" );
    }

    if ( err_code & ERR_CORRUPTED_CANARY ) {
        fprintf( stderr, "\tERROR: Corrupted canaries in the stack \n" );
    }

    if ( err_code & ERR_SIZE_OVER_CAPACITY ) {
        fprintf( stderr, "\tERROR: Size over capacity \n" );
    }

    if ( err_code & ERR_CORRUPTED_CAPACITY ) {
        fprintf( stderr, "\tERROR: Corrupted capacity \n" );
    }

    if ( err_code & ERR_POISON_IN_FILLED_CELLS ) {
        fprintf( stderr, "\tERROR: Poison in filled cells \n" );
    }

    if ( err_code & ERR_LARGE_CAPACITY ) {
        fprintf( stderr, "\tWARNING: Too big capacity \n" );
    }
}

long StackDump( Stack_t* stk ) {
    fprintf( stderr, "stack<int>[%p]" ON_DEBUG( " --- name: \"%s\" --- FILE: %s --- LINE: %lu --- FUNC: %s" ) "\n",
                     stk              ON_DEBUG( , stk->varInfo.name, stk->varInfo.file, stk->varInfo.line, stk->varInfo.func ) );
    ON_DEBUG(  \
        if (  stk->varInfo.err_code != ERR_NONE ) {
            fprintf( stderr, "  ERROR CODE %ld:\n", stk->varInfo.err_code );
            ErrorProcessing( stk->varInfo.err_code );
        }
    )

    fprintf( stderr, "  {              \n"
                     "  capacity  = %lu\n"
                     "  size      = %lu\n"
                     "  data[%lu] = %p \n",
                        stk->capacity,
                        stk->size,
                        stk->size, stk->ptr);

    if ( stk->varInfo.err_code & ( ERR_BAD_PTR_DATA ) ) {
        fprintf( stderr, "\t}\n" );
        return stk->varInfo.err_code;
    }

    size_t max_i = stk->capacity;

    if ( stk->varInfo.err_code & ERR_CORRUPTED_CAPACITY ) {
        if ( stk->size == 0 ) {
            fprintf( stderr, "\t}\n" );
            return stk->varInfo.err_code;
        }

        max_i = stk->size;
    }

    fprintf( stderr, "\t{\n");
    fprintf( stderr, "\t[can] = %d\n", *( stk->ptr - 1 ) );
    for ( size_t i = 0; i < max_i; i++ ) {
        if ( *( stk->ptr + i ) != poison ) {
            fprintf( stderr, "\t*[%lu] = %d\n", i, *( stk->ptr + i ) );
        }
        else {
            fprintf( stderr, "\t [%lu] = %d ( poison ) \n", i, *( stk->ptr + i ) );
        }
    }
    fprintf( stderr, "\t[can] = %d\n", *( stk->ptr + stk->capacity ) );
    fprintf( stderr, "\t}\n"
            "}\n" );

    ON_DEBUG( return stk->varInfo.err_code; )
    return ERR_NONE;
}
#endif

int* StackRealloc( Stack_t* stk, size_t capacity ) {
    stk->capacity = capacity;
    ON_DEBUG( stk->ptr--; )                      // canaries

    stk->ptr = ( int* ) realloc ( stk->ptr, ( capacity ON_DEBUG( + 2 ) ) * sizeof( int ) );         // canaries
    assert( stk->ptr != NULL );


    #ifdef _DEBUG                                  // canaries
           stk->canaries_ptr[0]   = stk->ptr;
        *( stk->canaries_ptr[0] ) = canary;

           stk->canaries_ptr[1]   = stk->ptr + stk->capacity;
        *( stk->canaries_ptr[1] ) = canary;

        stk->ptr++;
    #endif

    for ( size_t i = stk->size; i < capacity; i++ ) {
        *( stk->ptr + i ) = poison;
    }

    return stk->ptr;
}