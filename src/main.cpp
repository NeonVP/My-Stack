#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"


int main() {        // TODO: typedef ( если успею )
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
    stk1.size = 10000;
    ON_DEBUG( *stk1.canaries_ptr[0] = 1000; )

    StackPop ( &stk1 );
    CHECK_FOR_ERR( stk1 )
    ON_DEBUG( StackDump( &stk1 ); )

    StackPop ( &stk1 );
    CHECK_FOR_ERR( stk1 )
    ON_DEBUG( StackDump( &stk1 ); )


    StackDtor( &stk1 );
    fprintf( stderr, "%p \n", &stk1 );
}