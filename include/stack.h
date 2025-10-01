#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef int StackData_t;

#ifdef _CANARY
const int canary = 0xbabe;
#define ON_CANARY(...) __VA_ARGS__
#else
#define ON_CANARY(...)
#endif

#ifdef _DEBUG
#define ON_DEBUG(...) __VA_ARGS__
#define CHECK_FOR_ERR( stk ) if ( IS_CRITICAL_ERROR( stk.varInfo.err_code ) ) { StackDtor( &stk ); return 1; } else { stk.varInfo.err_code = ERR_NONE; }
#ifdef _CANARY
#define INIT( stk ) \
    canary, 0, 0, 0, { ERR_NONE, #stk, __func__, __LINE__, __FILE__ }, canary
#else
#define INIT( stk ) \
    0, 0, 0, { ERR_NONE, #stk, __func__, __LINE__, __FILE__ }
#endif //_CANARY
#define IS_CRITICAL_ERROR( code ) ( ( code ) & ( ERR_SIZE_OVER_CAPACITY | ERR_BAD_PTR_DATA | ERR_BAD_PTR_STRUCT | ERR_POISON_IN_FILLED_CELLS ) )
#define DEBUG_IN_FUNC(...)                  \
    if ( StackVerify( stk ) != ERR_NONE ) { \
        return;                           \
    }                                       \
    __VA_ARGS__                             \
                                            \
   StackVerify( stk );
#else
#define ON_DEBUG(...)
#define CHECK_FOR_ERR( code )
#ifdef _CANARY
#define INIT( stk ) \
    canary, 0, 0, 0, canary
#else
#define INIT( stk ) \
    0, 0, 0
#endif //_CANARY
#endif //_DEBUG

const int poison = 777;

enum ErrStack_t {
    ERR_NONE                    = 0,
    ERR_BAD_PTR_STRUCT          = 1 << 1,
    ERR_BAD_PTR_DATA            = 1 << 2,
    ERR_CORRUPTED_CANARY_DATA   = 1 << 3,
    ERR_CORRUPTED_CANARY_STRUCT = 1 << 4,
    ERR_SIZE_OVER_CAPACITY      = 1 << 5,
    ERR_LARGE_CAPACITY          = 1 << 6,
    ERR_CORRUPTED_CAPACITY      = 1 << 7,
    ERR_POISON_IN_FILLED_CELLS  = 1 << 8
};

struct VarInfo {
    long        err_code = ERR_NONE;
    const char* name     = 0;
    const char* func     = 0;
    size_t      line     = 0;
    const char* file     = 0;
};

struct Stack_t {
    ON_CANARY( int canary1 = canary; )           // canaries

    StackData_t* ptr        = 0;
    size_t size     = 0;
    size_t capacity = 0;

    ON_DEBUG( VarInfo varInfo = {}; )
    ON_CANARY( int canary2 = canary; )          // canaries
};

void         StackCtor( Stack_t* stk, size_t size );
void         StackDtor( Stack_t* stk );
void         StackPush( Stack_t* stk, int element );
StackData_t  StackPop ( Stack_t* stk );

StackData_t* StackRealloc( Stack_t* stk, size_t capacity );

long StackVerify( Stack_t* stk );
long StackDump  ( Stack_t* stk );

void ErrorProcessing( long err_code );

#endif //STACK_H