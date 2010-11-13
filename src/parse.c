#include <string.h>
#include "cmd.h"
#include "parse.h"
#include "memory.h"

char* ReadMorpheme(char* s,int *i){
    int size=0,maxsize=1;
    int ssize=strlen(s);
    char* res=NULL;
    res=(char*)myrealloc(res,maxsize);

    while (((*i)<ssize)&&(s[*i]==' ')) (*i)++;

    if ((*i)==ssize){
        free(res);
        return NULL;
    }

    while ((*i)<ssize && (s[*i]!=((size>0&&s[0]=='"')?'"':' ')) &&
           s[*i]!='#' && s[*i]!='&' && s[*i]!='|' &&
           s[*i]!=';' && s[*i]!='(' && s[*i]!=')'){
        if (++size>maxsize){
            maxsize*=2;
            res=(char*)myrealloc(res,maxsize);
        }
        res[size-1]=s[*i];
        (*i)++;
    }

    if (size!=0 && res[0]=='"') {
        strcpy(res,res+sizeof(char));
        size--;
    }
    if (size!=0 && res[size-1]=='"') {
        res[size-1]=0;
        size--;
    }

    if ((size==0) && ((*i)<ssize)){
        res[size++]=s[*i];
        (*i)++;
    }
    if (((*i)<ssize)&&((*i)>0)&&((s[(*i)-1]=='|')||(s[(*i)-1]=='&'))
       && (s[(*i)]==s[(*i)-1]&&(size==1))){
       res=(char*)myrealloc(res,2*sizeof(char));
       res[1]=s[(*i)++];
    }

    return res;
}

Cmd* ParseString(char *s){
    Cmd* cmd=NULL;
    char* morphem=NULL;
    States state=INIT_NODE;
    int len,slen,i=0;
    slen=strlen(s);
    while (i<slen){
        switch (state){
            case INIT_NODE:
                cmd=CmdInit();
                state=READ_CMD;
                break;

            case READ_CMD:
                morphem=ReadMorpheme(s,&i);
                len=strlen(morphem);
                if (len==1){
                    switch (morphem[0]){
                        case '(':
                            continue;
                            break;

                        default:
                            return NULL;
                            break;
                    }
                }
                break;
        }
    }
}
