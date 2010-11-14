#include "str.h"
#include "parse.h"
#include <stdio.h>


void testr(char* c){
    int i=0; Str* res; Err* err=ErrInit();
    c="ls -la | (grep asd >/dev/null -u || echo \"Hernya\" ) &";
    
    /*res=ParseLex(c,err,NULL);
    printf("%s\n",res->s);
    res=ParseLex(c+3,err,NULL);
    printf("%s\n",res->s);
    res=ParseLex(c+6,err,NULL);
    printf("%s\n",res->s);
    res=ParseLex(c+8,err,NULL);
    printf("%s\n",res->s);
    res=ParseLex(c+9,err,NULL);
    printf("%s\n",res->s);
    res=ParseLex(c+10,err,NULL);
    printf("%s\n",res->s);
    res=ParseLex(c+15,err,NULL);
    printf("%s\n",res->s);
    res=ParseLex(c+20,err,NULL);
    printf("%s\n",res->s);*/
    List* a=ParseBuildList(c,err);
    while (a!=NULL){
        printf("%s\n",a->data);
        a=a->next;
    }
}

void TestReadMorpheme(){
    char* c="abcde&&abd";
    int i=0; 
    i=0;
    c="ls -la | (grep asd >   /dev/null -u || echo \"Hernya\" ) &";
    c="asd || asd\0";
    testr(c);
}
