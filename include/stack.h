#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifdef _DEBUG
#define ON_DEBUG(...) __VA_ARGS__
#define CHECK_FOR_ERR( code ) if ( IS_ERROR( code ) ) { return 1; } else { code = ERR_NONE; }
#else
#define ON_DEBUG(...)
#define CHECK_FOR_ERR( code )
#endif //_DEBUG

#define INIT( stk ) \
     0, 0, 0 ON_DEBUG( , { ERR_NONE, #stk, __func__, __LINE__, __FILE__ } )

#define TWO_TAB "       "

#define   IS_ERROR( code ) ( ( code ) & ( ERR_SIZEOVERCAPACITY | ERR_BADPTR ) )
#define IS_WARNInG( code ) ( ( code ) & ( ERR_LARGECAPACITY ) )

enum ErrStack_t {
    ERR_NONE = 0,
    ERR_BADPTR = 1,
    ERR_SIZEOVERCAPACITY = 1 << 2,
    ERR_LARGECAPACITY = 1 << 3
};

struct VarInfo {
    long        err_code = ERR_NONE;
    const char* name     = 0;
    const char* func     = 0;
    size_t      line     = 0;
    const char* file     = 0;
};

struct Stack_t {
    int* ptr        = 0;
    size_t size     = 0;
    size_t capacity = 0;

    ON_DEBUG( VarInfo varInfo = {}; )
};



long StackCtor( Stack_t* stk, size_t size );
long StackDtor( Stack_t* stk );
long StackPush( Stack_t* stk, int element );
long StackPop ( Stack_t* stk );

long StackVerify( Stack_t* stk );
long StackDump  ( Stack_t* stk );

void ErrorProcessing( long err_code );

#endif //STACK_H