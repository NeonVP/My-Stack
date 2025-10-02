#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"


int main() {
    Stack_t stk1 = { INIT( stk1 ) };

    StackCtor( &stk1, 30 );
    CHECK_FOR_ERR( stk1 )

    StackPush( &stk1, 10 );
    CHECK_FOR_ERR( stk1 )
    StackPush( &stk1, 20 );
    CHECK_FOR_ERR( stk1 )

    StackPush( &stk1, 30 );
    CHECK_FOR_ERR( stk1 )

    StackPush( &stk1, 40 );
    CHECK_FOR_ERR( stk1 )

    /* ---- Check StackVerify ---- */
    ON_DEBUG( stk1.size = 10000; )
    ON_CANARY( *( stk1.data - 1 ) = 0xcafe; )

    StackPop ( &stk1 );
    CHECK_FOR_ERR( stk1 )
    ON_DEBUG( StackDump( &stk1 ); )

    StackPop ( &stk1 );
    CHECK_FOR_ERR( stk1 )
    ON_DEBUG( StackDump( &stk1 ); )

    StackDtor( &stk1 );
}