#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifdef _DEBUG
#define ON_DEBUG(...) __VA_ARGS__
#else
#define ON_DEBUG(...)
#endif //_DEBUG

#define TWO_TAB "       "

enum ErrStack_t {
    ERR_NONE = 0,
    ERR_BADPTR = 2,
    ERR_SIZEOVERCAPACITY = 4,
    ERR_LARGECAPACITY = 8
};

struct VarInfo {
    long err_code = ERR_NONE;
    const char* name = 0;
    const char* func = 0;
    size_t      line = 0;
    const char* file = 0;
};

struct Stack_t {
    int* ptr        = 0;
    size_t size     = 0;
    size_t capacity = 0;
    ON_DEBUG( VarInfo varInfo = {}; )
};

#define INIT( stk ) \
     0, 0, 0 ON_DEBUG( , { ERR_NONE, #stk, __func__, __LINE__, __FILE__ } )

ErrStack_t StackCtor( Stack_t* stk, size_t size );
ErrStack_t StackDtor( Stack_t* stk );

ErrStack_t StackPush( Stack_t* stk, int element );
ErrStack_t StackPop ( Stack_t* stk );

ErrStack_t StackVerify( Stack_t* stk );
ErrStack_t StackDump  ( Stack_t* stk );

void StackPrint( Stack_t stk );
void StackStat( Stack_t stk );

#endif //STACK_H