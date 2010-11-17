#include "tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "str.h"
#include "memory.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int file_exists(char * fileName){
   struct stat buf;
   int i = stat ( fileName, &buf );
   if ( i == 0 ){
       return 1;
   }
   perror("file_exists:");
   return 0;
}


void ExecError(char* s){
    fprintf(stderr,"%s\n",s);
}

char **BuildPtr(char* cmd, List* args){
    int i=0,len=0;
    char** s=NULL;
    List* L=args;
    while (L!=NULL){
        i++;
        L=L->next;
    }
    s=(char**)myrealloc(s,i+2);
    len=i;
    StringPut(&(s[0]),(char*)cmd);
    for (i=1; i<=len; i++) s[i]=NULL;
    i=1; L=args;
    while (L!=NULL){
        StringPut(&(s[i]),(char*)L->data);
        i++;
        L=L->next; 
    }
    return s;    
}

int ExecSetRedirections(Tree* T){
    int innew,outnew;
    if (T->in!=NULL){
        if (file_exists(T->in)){
            T->insaved=dup(0);
            innew=open(T->in,O_RDONLY);
            if (innew<0){ perror("ExecSetRedirections"); return 1;}
            if (dup2(innew,0)<0) { perror("ExecSetRedirections"); return 1;}
            close(innew);
        }
    }
    if (T->out!=NULL && T->append==NULL){
        outnew=open(T->out,O_WRONLY|O_CREAT,0666);
        T->outsaved=dup(1);
        if (outnew<0){ perror("ExecSetRedirections"); return 1;}
        if (dup2(outnew,1)<0) { perror("ExecSetRedirections"); return 1;}
        close(outnew);
        return 0;
    }
    if (T->out!=NULL && T->append!=NULL) ExecError(">> and > cannot be set"
            "at once. Using >>.");
    if(T->append!=NULL){
        outnew=open(T->out,O_WRONLY|O_CREAT|O_APPEND,0666);
        T->outsaved=dup(1);
        if (outnew<0){ perror("ExecSetRedirections"); return 1;}
        if (dup2(outnew,1)<0) { perror("ExecSetRedirections"); return 1;}
        close(outnew);
        return 0;
    }
    return 0;
}

int ExecKillRedirections(Tree* T){
    if (T->insaved!=-1){
        close(0);
        if (dup2(T->insaved,0)<0) { perror("ExecKillRedirections"); return 1;}
        close(T->insaved);
        T->insaved=-1;
    }
    if (T->outsaved!=-1){
        close(1);
        if (dup2(T->outsaved,1)<0) { perror("ExecKillRedirections"); return 1;}
        close(T->outsaved);
        T->outsaved=-1;
    }
    return 0;
}

int ExecuteCmd(char* c){
    Err* E=ErrInit();
    Tree* T=ParseFull(c,E);
    if (E->pres==1){
        ExecError("Can not run command");
        ExecError(E->err);
    } else {
        return ExecuteTree(T);
    }
}

int ExecuteTree(Tree* T){
    char* s;
    int status;
    switch (T->type){
        case LINK_COMMAND:
            if (ExecSetRedirections(T)!=0)
                ExecError("Can not set redirections! Continuing w/o, be care!\n");
            switch (fork()){
                case -1:
                    perror("Forking Command");
                case 0:
                    //printf("i'm child %s!\n",T->cmd);
                    //printf("baba %s\n",(BuildPtr(T->cmd,T->args))[0]);
                    execvp(T->cmd,BuildPtr(T->cmd,T->args));
                    perror("fork");
                    ExecError("Cannot fork to start program!");
                    break;
                default:
                    if (ExecKillRedirections(T)!=0){
                        ExecError("Can not unset redirections >_< Exiting");
                        _exit(1);
                    }
                    wait(&status);
                    return status;
            }
            break;
        case LINK_SUBSHELL:
            if (ExecSetRedirections(T)!=0)
                ExecError("Can not set redirections! Continuing w/o, be care!\n");
            if (T->left==NULL || T->right!=NULL){
                ExecError("Unknown algorithm error!");
                return 1;
            }
            else {
                status=ExecuteTree(T->left);
            }
            if (ExecKillRedirections(T)!=0){
                ExecError("Can not unset redirections >_< Exiting");
                _exit(1);
            }
            return status;
    }
    return 0;
}
