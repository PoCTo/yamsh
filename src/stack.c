#include "stack.h"
#include "memory.h"

Tree* TreeInit(){
    Tree* T;
    T=myrealloc(T,sizeof(Tree));
    T->args=NULL; T->in=NULL; T->out=NULL;
    T->delim=LINK_NULL;
    T->left=NULL; T->right=NULL;
    T->cmd=NULL;
    return T;
}