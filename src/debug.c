#include "debug.h"

void printnode(Tree* T, FILE* f){
    if (T==NULL) {printf("NULL"); return; }
    fprintf(f,"node%d [shape=",(int)T);
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
    if (T->left!=NULL)
        fprintf(f,"node%d -> node%d;\n",(int)T,(int)T->left);
    if (T->right!=NULL)
        fprintf(f,"node%d -> node%d;\n",(int)T,(int)T->right);
    printtree(T->left,f);
    printtree(T->right,f);
    fclose(f);
}

void printcommandtree(char* cmd, char* file){
    Err* E=ErrInit();
    FILE *f;
    Tree* T=ParseFull(cmd,E);
    if (E->pres==0) ErrFree(E);
    else {
        f=fopen(file,"w");
        if (f<0) perror("Debug file open");
        fprintf(f,"%s\n",E->err);
        ErrFree(E);
        fclose(f);
        return;
    }
    f=fopen(file,"w");
    if (f<0) perror("Debug file open");
    printtree(T,file);
    TreeFree(T);
    fclose(f);
}
