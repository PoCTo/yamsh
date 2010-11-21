#include "debug.h"

void printnode(Tree* T, FILE* f){
    if (T==NULL) {printf("NULL"); return; }
    fprintf(f,"node%p [shape=",T);
    switch (T->type){
        case LINK_SUBSHELL:
            fprintf(f,"box, label=\"Subshell\"];\n");
            break;
        case LINK_COMMAND:
            fprintf(f,"ellipse, label=%s];\n",T->cmd);
            break;
        case LINK_NULL:
            fprintf(f,"ellipse, label=\"NULL\"];\n");
            break;
        case LINK_AND:
            fprintf(f,"diamond, label=\"&&\"];\n");
            break;
        case LINK_OR:
            fprintf(f,"diamond, label=\"||\"];\n");
            break;
        case LINK_PIPE:
            fprintf(f,"diamond, label=\"|\"];\n");
            break;
        case LINK_SEMICOLON:
            fprintf(f,"diamond, label=\";\"];\n");
            break;
        case LINK_BACKGROUND:
            fprintf(f,"diamond, label=\"&\"];\n");
            break;
    }

}

void printtree(Tree* T, FILE* f){
    
    if (T==NULL){ return; }
    printnode(T,f);
    if ((T->left)!=NULL)
        fprintf(f,"node%p -> node%p;\n",T,(T->left));
    if ((T->right)!=NULL)
        fprintf(f,"node%p -> node%p;\n",T,(T->right));
    printtree(T->left,f);
    printtree(T->right,f);
    //fclose(f);
}

void printcommandtree(char* cmd){
    Err* E=ErrInit();
    FILE *f=NULL;
    Tree* T=ParseFull(cmd,E);
    if ((E->pres)==0) ErrFree(E);
    else {
        f=fopen("/tmp/shell.dot","w");
        //if (f<0) perror("Debug file open");
        printf("%s\n",E->err);
        ErrFree(E);
        fclose(f);
        return;
    }
    f=fopen("/tmp/shell.dot","w");
    //if (f<0) perror("Debug file open");
    fprintf(f,"Digraph mega{\n");
    printtree(T,f);
    TreeFree(T);
    fprintf(f,"}\n");
    fclose(f);
}
