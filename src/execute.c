#include "execute.h"

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
    s=(char**)myrealloc(s,(i+2)*sizeof(char*));
    len=i+2;
    s[0]=NULL;
    
    for (i=1; i<=len; i++) s[i]=NULL;
    i=1; L=args;
    while (L!=NULL){
        StringPut(&(s[i]),(char*)L->data);
        i++;
        L=L->next; 
    }
    StringPut(&(s[0]),cmd);
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

int ExecSetWritePipe(Tree* T, int p[2]){
    T->outsaved=dup(1);
    if (dup2(p[1],1)<0) { perror("ExecSetWritePipe"); return 1;}
    close(p[0]);
    close(T->outsaved);
    return 0;
}
int ExecSetReadPipe(Tree* T, int p[2]){
    T->insaved=dup(0);
    if (dup2(p[0],0)<0) { perror("ExecSetWritePipe"); return 1;}
    close(p[1]);
    close(T->insaved);
    return 0;
}

int ExecuteCmd(char* c){
    Err* E=ErrInit();
    Tree* T=ParseFull(c,E);
    if (E->pres==1){
        ExecError("Can not run command");
        ExecError(E->err);
        return -1;
    } else {
        return ExecuteTree(T);
    }
    TreeFree(T);
}

int ExecuteTree(Tree* T){
    int p[2];
    pid_t pid,pid1,pid2;
    int i=0;
    int status=0,stat1=0,stat2=0;
    char **argsptr;
    
    switch (T->type){
        case LINK_COMMAND:
            if (ExecSetRedirections(T)!=0)
                ExecError("Can not set redirections! Continuing w/o, be care!\n");
            argsptr=BuildPtr(T->cmd,T->args);
            switch (fork()){
                case -1:
                    perror("Forking Command");
                    break;
                case 0:
                    //printf("i'm child %s!\n",T->cmd);
                    //printf("baba %s\n",(BuildPtr(T->cmd,T->args))[0]);
                    execvp(T->cmd,argsptr);
                    perror("exec");
                    ExecError("Cannot fork to start program!");
                    break;
                default:
                    if (ExecKillRedirections(T)!=0){
                        ExecError("Can not unset redirections >_< Exiting");
                        _exit(1);
                    }
                    wait(&status);
                    //fprintf(stderr,"%s,%s\n",*(&(argsptr[0])),argsptr[1]);
                    //for (i=0;(argsptr[i])!=NULL;i++) free(argsptr[i]);
                    //free(argsptr);
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
        case LINK_AND:
            stat1=ExecuteTree(T->left);
            if (stat1==0) stat2=ExecuteTree(T->right);
            return !(!stat1 && !stat2);
        case LINK_OR:
            stat1=ExecuteTree(T->left);
            if (stat1!=0) stat2=ExecuteTree(T->right);
            return !(!stat1 || !stat2);
        case LINK_SEMICOLON:
            stat1=ExecuteTree(T->left);
            stat2=ExecuteTree(T->right);
            return stat2;
        case LINK_PIPE:
            if (T->left!=NULL) {
                pipe(p);
                switch (pid1=fork()){
                    case -1:
                        perror("Forking Command");
                        break;
                    case 0: //son
                        ExecSetWritePipe(T,p);
                        _exit(ExecuteTree(T->left));
                        break;
                    default: //father
                        switch(pid2=fork()){
                            case -1:
                               perror("Forking Command");
                               break;
                            case 0:
                                if (T->right==NULL) return 0;
                                ExecSetReadPipe(T,p);
                                _exit(ExecuteTree(T->right));
                                break;
                            default: //father
                                close(p[1]); close(p[0]);
                                for (i=0; i<2; i++){
                                    pid=wait(&status);
                                    if (pid==-1) {
                                        perror("Exec Pipe");
                                        break;
                                    } else if (pid==pid1){
                                        stat1=status;
                                    } else if (pid==pid2){
                                        stat2=status;
                                    } else {
                                        ExecError("WTF? Another Process there");
                                    }
                                }
                                return stat2;                                
                        }
                }
            }
        case LINK_BACKGROUND:
            switch (pid1=fork()){
                case -1:
                    perror("Exec B-G");
                    break;
                case 0:
                    /*switch (pid2=fork()){
                        case -1:
                            perror("Exec B-G");
                            break;
                        case 0:
                            if (T->left!=NULL) exit(ExecuteTree(T->left));
                            else exit(0);
                            break;
                        default:
                            fprintf(stderr, "[BG] Spawned process [%d]\n",pid);
                            wait(&status);
                            fprintf(stderr,"[BG] Child [%d] exited with code %d\n",pid,status);
                            break;
                    }
                    break;*/
                    fprintf(stderr, "[BG] Spawned process [%d]\n",pid);
                    if (T->left!=NULL) status=ExecuteTree(T->left);
                    else status=0;
                    fprintf(stderr,"[BG] Child [%d] exited with code %d\n",pid,status);
                    exit(status);
                default:
                    if (T->right!=NULL) return ExecuteTree(T->right);
                    else return 0;
                    break;
            }
            break;
    }
    return 0;
}
