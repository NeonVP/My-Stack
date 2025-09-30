#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifdef _DEBUG
const int canary = 0xEDA;
#define ON_DEBUG(...) __VA_ARGS__
#define CHECK_FOR_ERR( stk ) if ( IS_CRITICAL_ERROR( stk.varInfo.err_code ) ) { StackDtor( &stk ); return 1; } else { stk.varInfo.err_code = ERR_NONE; }
#define INIT( stk ) \
    0, 0, 0, { ERR_NONE, #stk, __func__, __LINE__, __FILE__ }, { 0, 0 }
#define IS_CRITICAL_ERROR( code ) ( ( code ) & ( ERR_SIZE_OVER_CAPACITY | ERR_BAD_PTR_DATA | ERR_BAD_PTR_STRUCT | ERR_POISON_IN_FILLED_CELLS ) )
#else
#define ON_DEBUG(...)
#define CHECK_FOR_ERR( code )
#define INIT( stk ) \
    0, 0, 0
#endif //_DEBUG

const int poison = 777;

enum ErrStack_t {
    ERR_NONE                   = 0,
    ERR_BAD_PTR_STRUCT         = 1 << 1,
    ERR_BAD_PTR_DATA           = 1 << 2,
    ERR_CORRUPTED_CANARY       = 1 << 3,
    ERR_SIZE_OVER_CAPACITY     = 1 << 4,
    ERR_LARGE_CAPACITY         = 1 << 5,
    ERR_CORRUPTED_CAPACITY     = 1 << 6,
    ERR_POISON_IN_FILLED_CELLS = 1 << 7
};

struct VarInfo {
    long        err_code = ERR_NONE;
    const char* name     = 0;
    const char* func     = 0;
    size_t      line     = 0;
    const char* file     = 0;
};

struct Stack_t {    // TODO: canaries on structure
    // ON_DEBUG( int canary1     = canary; )           // canaries

    int* ptr       = 0;
    size_t size     = 0;
    size_t capacity = 0;

    ON_DEBUG( VarInfo varInfo = {};      )
    ON_DEBUG( int* canaries_ptr[2] = {}; )          // canaries
    // ON_DEBUG( int canary2 = canary;      )          // canaries
};



void StackCtor( Stack_t* stk, size_t size );
void StackDtor( Stack_t* stk );
void StackPush( Stack_t* stk, int element );
int  StackPop ( Stack_t* stk );

int* StackRealloc( Stack_t* stk, size_t capacity );

long StackVerify( Stack_t* stk );
long StackDump  ( Stack_t* stk );

void ErrorProcessing( long err_code );

#endif //STACK_H