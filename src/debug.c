#include "debug.h"

void printnode(Tree* T, FILE* f){
    if (T==NULL) {printf("NULL"); return; }
    printf("node%d [shape=",(int)T);
    switch (T->type){
        case LINK_SUBSHELL:
            printf("box, label=\"Subshell\"];\n");
            break;
        case LINK_COMMAND:
            printf("ellipse, label=%s];\n",T->cmd);
            break;
        case LINK_NULL:
            printf("ellipse, label=\"NULL\"];\n");
            break;
        case LINK_AND:
            printf("diamond, label=\"&&\"];\n");
            break;
        case LINK_OR:
            printf("diamond, label=\"||\"];\n");
            break;
        case LINK_PIPE:
            printf("diamond, label=\"|\"];\n");
            break;
        case LINK_SEMICOLON:
            printf("diamond, label=\";\"];\n");
            break;
        case LINK_BACKGROUND:
            printf("diamond, label=\"&\"];\n");
            break;
    }

}

void printtree(Tree* T, FILE* f){
    
    if (T==NULL){ return; }
    printnode(T,f);
    if ((T->left)!=NULL)
        printf("node%d -> node%d;\n",(int)T,(int)(T->left));
    if ((T->right)!=NULL)
        printf("node%d -> node%d;\n",(int)T,(int)(T->right));
    printtree(T->left,f);
    printtree(T->right,f);
    //fclose(f);
}

void printcommandtree(char* cmd, char* file){
    Err* E=ErrInit();
    //FILE *f=NULL;
    Tree* T=ParseFull(cmd,E);
    if ((E->pres)==0) ErrFree(E);
    else {
        //f=fopen(file,"w");
        //if (f<0) perror("Debug file open");
        printf("%s\n",E->err);
        ErrFree(E);
        //fclose(f);
        return;
    }
    //f=fopen(file,"w");
    //if (f<0) perror("Debug file open");
    printf("Digraph mega{\n");
    printtree(T,NULL);
    TreeFree(T);
    printf("}\n");
    //fclose(f);
}
