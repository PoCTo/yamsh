#include "str.h"
#include "parse.h"
#include <stdio.h>
#include "tree.h"


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

void printnode(Tree* T){
    if (T==NULL) {printf("NULL"); }
    switch (T->type){
        case LINK_SUBSHELL:
            printf("Subshell\n");
            break;
        case LINK_COMMAND:
            printf("Command: %s\n",T->cmd);
            break;
        case LINK_NULL:
            printf("Error!\n");
            break;
        case LINK_AND:
            printf("&&\n");
            break;
        case LINK_OR:
            printf("||\n");
            break;
        case LINK_PIPE:
            printf("|\n");
            break;
        case LINK_SEMICOLON:
            printf(";\n");
            break;
        case LINK_BACKGROUND:
            printf("&\n");
            break;
    }

}

void printtree(Tree* T){
    if (T==NULL){ return; }
    printnode(T);
    printf("Sons:{");
    printnode(T->left);
    printnode(T->right);
    printf("}");
}

void TestTree(){
    char *c="ls -la | (grep asd >   /dev/null -u || echo \"Hernya\" ) &\0";
    Err* E=ErrInit();
    List* L=ParseBuildList(c,E);
    if (E->pres==1) printf("Fuck");
    Tree* T=ParseBuildTree(L,E);
    if (E->pres!=1) printtree(T); else printf("Fuck2");
}
