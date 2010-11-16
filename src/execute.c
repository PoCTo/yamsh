#include "tree.h"

char **BuildPtr(char* cmd, List* args){
    int i=1;
    List* L=args;
    //while (args!=NULL){
        
    //}
    
}

int Execute(Tree* T){
    switch (T->type){
        case LINK_COMMAND:
            execvp(T->cmd,BuildPtr(T->cmd,T->args));
    }
    return 0;
}
