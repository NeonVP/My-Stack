#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"


int main() {
    Stack_t stk1 = { INIT(stk1) };
    StackCtor( &stk1, 20 );
    printf( "%p - %lu - %lu\n", stk1.ptr, stk1.size, stk1.capacity );

    StackPush( &stk1, 10 );
    StackPush( &stk1, 20 );
    StackDump( &stk1 );

    StackPush( &stk1, 30 );
    StackPush( &stk1, 40 );
    StackDump( &stk1 );

    StackPop( &stk1 );
    StackDump( &stk1 );

    StackDtor( &stk1 );
}