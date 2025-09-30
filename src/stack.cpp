#include "stack.h"

long StackCtor( Stack_t* stk, size_t capacity ) {
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

    ON_DEBUG( return StackVerify( stk ); )
    return ERR_NONE;
}

long StackDtor( Stack_t* stk ) {            // TODO: change StackDtor
    free( --stk->ptr );
    // free( stk );

    return ERR_NONE;
}

long StackPush( Stack_t* stk, int element ) {
    assert( stk != NULL );

    ON_DEBUG(
        if ( StackVerify( stk ) != ERR_NONE ) {
            return stk->varInfo.err_code;
        }
    )

    if ( stk->size + 1 > stk->capacity ) {
        stk->capacity *= 2;
        int* new_ptr = ( int* ) realloc ( stk->ptr, ( stk->capacity ON_DEBUG( + 2 ) ) * sizeof( int ) );
        assert( new_ptr != NULL );

        stk->ptr = new_ptr;
    }

    *( stk->ptr + stk->size ) = element;
    stk->size++;

    ON_DEBUG( StackVerify( stk ); )
    return ERR_NONE;
}

long StackPop( Stack_t* stk ) {
    ON_DEBUG(
        if ( StackVerify( stk ) != ERR_NONE ) {
            return stk->varInfo.err_code;
        }
    )

    stk->size--;
    *( stk->ptr + stk->size ) = poison;

    if ( stk->size * 4 <= stk->capacity ) {
        // stk->capacity /= 2;
        // stk->ptr = ( int* ) realloc ( stk->ptr, stk->capacity * sizeof( int ) );
        // assert( stk->ptr != NULL );

        StackRealloc( stk, stk->capacity / 2 );
    }

    ON_DEBUG( return StackVerify( stk ); )
    return ERR_NONE;
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
        fprintf( stderr, "    ERROR: INVALID pointer to the structure with parameters  \n" );
        return;
    }

    if ( err_code & ERR_BAD_PTR_DATA ) {
        fprintf( stderr, "    ERROR: INVALID pointer to the stack \n" );
    }

    if ( err_code & ERR_CORRUPTED_CANARY ) {
        fprintf( stderr, "    ERROR: Corrupted canaries in the stack \n" );
    }

    if ( err_code & ERR_SIZE_OVER_CAPACITY ) {
        fprintf( stderr, "    ERROR: Size over capacity \n" );
    }

    if ( err_code & ERR_LARGE_CAPACITY ) {
        fprintf( stderr, "    WARNING: Too big capacity \n" );
    }

    if ( err_code & ERR_CORRUPTED_CAPACITY ) {
        fprintf( stderr, "    ERROR: Corrupted capacity \n" );
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

           stk->canaries_ptr[1]   = stk->ptr + stk->capacity + 1;
        *( stk->canaries_ptr[1] ) = canary;

        stk->ptr++;
    #endif

    for ( size_t i = stk->size; i < capacity; i++ ) {
        *( stk->ptr + i ) = poison;
    }

    return stk->ptr;
}