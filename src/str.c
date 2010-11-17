#include "str.h"

Str* StrInit(){
    Str* S=NULL;
    S=myrealloc(S,sizeof(Str));
    S->s=NULL;
    S->s=myrealloc(S->s,sizeof(char));
    S->len=0;
    S->mem=1;
    return S;
}

Err* ErrInit(){
    Err* S=NULL;
    S=myrealloc(S,sizeof(Err));
    S->err=NULL;
    S->pres=0;
    return S;
}

void ErrFree(Err* E){
    myfree(E->err);
    myfree(E);
}

void StrPutChar(Str* S,char c){
    if ((S->len)+1>(S->mem)){
        (S->mem)*=2;
        S->s=myrealloc(S->s,(S->mem)*sizeof(char));
    }
    (S->s)[((S->len)++)]=c;
}

void StrClear(Str* S){
    S->len=0;
    S->mem=0;
    myfree(S->s);
    myfree(S);
}

char StrLast(Str* S){
    return (S->s)[(S->len)-1];
}

void StringPut(char** dest,char* src){
    *dest=(char*)myrealloc(*dest,strlen(src)*sizeof(char));
    strcpy(*dest,src);
}

void StrFree(Str* S){
    myfree(S->s); myfree(S);
}
