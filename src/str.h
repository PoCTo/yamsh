#ifndef STR_H
#define	STR_H

#include "memory.h"
#include <string.h>

typedef struct Str{
    char* s;
    int mem,len;
} Str;

typedef struct Err{
    int pres;
    char* err;
}Err;

Str* StrInit();
Err* ErrInit();
void ErrFree(Err* E);
void StrPutChar(Str* S, char c);
char StrLast(Str* S);
void StrFree(Str* S);
void StringPut(char** dest,char* src);

#endif	/* STR_H */

