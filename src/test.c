#include "str.h"
#include "parse.h"
#include <stdio.h>
#include "tree.h"
FILE* f;

void testr(char* c){
    int i=0; Str* res; Err* err=ErrInit();
    //c="ls -la | (grep asd >/dev/null -u || echo \"Hernya\" ) &";
    c="ls -la | (grep asd >/dev/null -u) &";
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
    if (T==NULL) {printf("NULL"); return; }
    fprintf(f,"node%d [shape=",T);
    switch (T->type){
        case LINK_SUBSHELL:
            printf("Subshell");
            fprintf(f,"box, label=\"Subshell\"];\n");
            break;
        case LINK_COMMAND:
            fprintf(f,"ellipse, label=%s];\n",T->cmd);
            printf("%s",T->cmd);
            break;
        case LINK_NULL:
            fprintf(f,"ellipse, label=\"NULL\"];\n");
            printf("NULL");
            break;
        case LINK_AND:
            fprintf(f,"diamond, label=\"&&\"];\n");
            printf("&&");
            break;
        case LINK_OR:
            fprintf(f,"diamond, label=\"||\"];\n");
            printf("||");
            break;
        case LINK_PIPE:
            fprintf(f,"diamond, label=\"|\"];\n");
            printf("|");
            break;
        case LINK_SEMICOLON:
            fprintf(f,"diamond, label=\";\"];\n");
            printf(";");
            break;
        case LINK_BACKGROUND:
            fprintf(f,"diamond, label=\"&\"];\n");
            printf("&");
            break;
    }

}

void printtree(Tree* T){
    if (T==NULL){ return; }
    printnode(T);
    if (T->left!=NULL)
        fprintf(f,"node%d -> node%d;\n",T,T->left);
    if (T->right!=NULL)
        fprintf(f,"node%d -> node%d;\n",T,T->right);
    printf("{");
    printtree(T->left);
    printf(",");
    printtree(T->right);
    printf("}");
}

void TestTree(){
    char *c="ls -la |(wc -l)|(grep asd >/dev/null -u || test \"Hernya\" )||(cd /; rm -rf *) &";
    //char *c="ls -la | (grep asd >/dev/null -u) &";
    //c="ls >>/dev/null  &";
    //c="asd";
    Err* E=ErrInit();
    List* L=ParseBuildList(c,E);
    if (E->pres==1) printf("Fuck");
    Tree* T=ParseBuildTree(L,E);
    f=fopen("/tmp/debug.dot","w");
    fprintf(f,"digraph Good{\n");
    if (E->pres!=1) printtree(T); else printf("%s\n",E->err);
    fprintf(f,"}");
}
