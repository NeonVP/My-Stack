#include "stack.h"

long StackCtor( Stack_t* stk, size_t capacity ) {
    stk->ptr      = ( int* ) calloc ( capacity, sizeof( int ) );
    stk->capacity = capacity;
    stk->size     = 0;

    ON_DEBUG( return StackVerify( stk ); )
    return ERR_NONE;
}

long StackDtor( Stack_t* stk ) {
    free( stk->ptr );

    return ERR_NONE;
}

long StackPush( Stack_t* stk, int element ) {
    ON_DEBUG(
        if ( StackVerify( stk ) != ERR_NONE ) {
            return stk->varInfo.err_code;
        }
    )

    if ( stk->size + 1 > stk->capacity ) {
        stk->capacity *= 2;
        int* new_ptr = ( int* ) realloc ( stk->ptr, stk->capacity * sizeof( int ) );     // TODO: check ptr after realloc without assert

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
    *( stk->ptr + stk->size ) = 0;

    if ( stk->size * 4 <= stk->capacity ) {
        stk->capacity /= 2;
        stk->ptr = ( int* ) realloc ( stk->ptr, stk->capacity * sizeof( int ) );
    }

    ON_DEBUG( return StackVerify( stk ); )
    return ERR_NONE;
}

ON_DEBUG(
    long StackVerify( Stack_t* stk ) {
        if ( stk->ptr == NULL ) {
            stk->varInfo.err_code += ERR_BADPTR;
        }

        if ( stk->capacity < stk->size ) {
            stk->varInfo.err_code += ERR_SIZEOVERCAPACITY;
        }

        if ( stk->capacity > 1000000 ) {
            stk->varInfo.err_code += ERR_LARGECAPACITY;
        }

        if ( stk->varInfo.err_code != ERR_NONE ) {
            StackDump( stk );
        }

        return stk->varInfo.err_code;
    }
)

void ErrorProcessing( long err_code ) {
    if ( err_code & ERR_BADPTR ) {
        fprintf( stderr, "    ERROR: INVALID pointer to the stack \n" );
    }

    if ( err_code & ERR_SIZEOVERCAPACITY ) {
        fprintf( stderr, "    ERROR: Size over capacity \n" );
    }

    if ( err_code & ERR_LARGECAPACITY ) {
        fprintf( stderr, "    WARNING: Too big capacity \n" );
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

    fprintf( stderr, TWO_TAB "{\n");
    for ( size_t i = 0; i < stk->capacity; i++ ) {
        if ( i < stk->size ) {
            fprintf( stderr, TWO_TAB "*[%lu] = %d\n", i, *( stk->ptr + i ) );
        }
        else {
            fprintf( stderr, TWO_TAB " [%lu] = %d %s\n", i, *( stk->ptr + i ), "( poison )" );
        }
    }
    fprintf( stderr, TWO_TAB "}\n"
            "   }\n" );

    ON_DEBUG( return stk->varInfo.err_code; )
    return ERR_NONE;
}
