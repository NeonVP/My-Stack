#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef _DEBUG
#define ON_DEBUG(...) __VA_ARGS__
#else
#define ON_DEBUG(...)
#endif

struct Stack_t {
    int* ptr = 0;
    size_t size = 0;
    size_t capacity = 0;
};

void StackCtor( Stack_t* stk, size_t size );
void StackPush( Stack_t* stk, int element );
void StackPop ( Stack_t* stk );

void StackPrint( Stack_t stk );
void StackStat( Stack_t stk );

int main() {
    Stack_t stk1 = {};
    StackCtor( &stk1, 10 );
    printf( "%p - %lu - %lu\n", stk1.ptr, stk1.size, stk1.capacity );

    StackPush( &stk1, 11 );
    StackPush( &stk1, 22 );
    StackPrint( stk1 );

    StackPop( &stk1 );
    StackPrint( stk1 );

    free( stk1.ptr );
}

void StackCtor( Stack_t* stk, size_t capacity ) {
    assert( stk != NULL );

    stk->ptr = ( int* ) calloc ( capacity, sizeof( int ) );
    stk->capacity = capacity;
}

void StackPush( Stack_t* stk, int element ) {
    if ( stk->size + 1 > stk->capacity ) {
        stk->ptr = ( int* ) realloc ( stk->ptr, stk->capacity * 2 );
        stk->capacity *= 2;
    }

    *( stk->ptr + stk->size++ ) = element;
}

void StackPop( Stack_t* stk ) {
    *( stk->ptr + --(stk->size) ) = 0;
    
    if ( stk->size * 4 <= stk->capacity ) {
        stk->capacity /= 2;
        stk->ptr = ( int* ) realloc ( stk->ptr, stk->capacity );
    }
}

void StackPrint( Stack_t stk ) {
    StackStat( stk );

    for ( size_t i = 0; i < stk.size; i++ ) {
        fprintf(  stderr, "stk[%lu] = %d\n", i, *( stk.ptr + i ) );
    }

    printf( "\n\n" );
}

void StackStat( Stack_t stk ) {
    printf( "StackStat: ptr      - %p\n"
            "           size     - %lu\n"
            "           capacity - %lu\n\n",
            stk.ptr, stk.size, stk.capacity );
}

