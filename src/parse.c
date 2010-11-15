#include <string.h>
#include "parse.h"
#include "memory.h"
#include "tree.h"
#include "str.h"
#include "list.h"
#include <stdio.h>

char* ReadMorpheme(char* s,int *i){
    int size=0,maxsize=1;
    int ssize=strlen(s);
    char* res=NULL;
    res=(char*)myrealloc(res,maxsize);

    while (((*i)<ssize)&&(s[*i]==' ')) (*i)++;

    if ((*i)==ssize){
        myfree(res);
        return NULL;
    }

    while ((*i)<ssize && (s[*i]!=((size>0&&s[0]=='"')?'"':' ')) &&
           s[*i]!='#' && s[*i]!='&' && s[*i]!='|' &&
           s[*i]!=';' && s[*i]!='(' && s[*i]!=')'
           && s[*i]!='>' && s[*i]!='<'){
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
    if (((*i)<ssize)
       &&((*i)>0)&&((s[(*i)-1]=='|')||(s[(*i)-1]=='&')||(s[(*i-1)]=='>'))
       && (s[(*i)]==s[(*i)-1]&&(size==1))){
       res=(char*)myrealloc(res,2*sizeof(char));
       res[1]=s[(*i)++];
    }

    return res;
}

/*Tree* ParseString(char *s){
    Tree* T=TreeInit();
    int i=0;
    ParseCommand(T,s,&i);
}*/

/*int isoper(char* t){
    if (strlen(t)>2) return 0;
    if (strlen(t)==0) return 0;
    if ((strlen(t)==1)&&(t[0]=='|' || t[0]==';'))
}*/

int CharSimpleOp(char c){
    return (c=='(')||(c=='<')||(c==')')||(c==';');
}
int CharDoubleOp(char c){
    return (c=='&')||(c=='>')||(c=='|');
}
int CharOp(char c){
    return CharSimpleOp(c)||CharDoubleOp(c);
}

Str* ParseLex(char* s,Err* err,int* i){
    int len=strlen(s);
    Str* S=NULL;
    S=StrInit();
    int noStep=0;
    char c;

    ParseLexStates prevstate,state=BLANK;
    while ((*i)<len && s[*i]==' ') (*i)++;
    for (;(state!=ERROR && (*i)<=len);){
        c=s[*i];
        prevstate=state;
        switch (state){
            case BLANK:
                if (c=='"') state=QUOTE;
                else if (CharOp(c)) {
                    state=OPERATOR;
                    StrPutChar(S,c);
                } else if (c==' ') state=BLANK;
                else {
                    StrPutChar(S,c);
                    state=WORD;
                }
                break;
            case QUOTE:
                if (c!='"') StrPutChar(S,c);
                if (c=='"') {
                    StrPutChar(S,'\0');
                    state=END;
                    (*i)++;
                } else if ((*i)==len-1) state=ERROR;
                break;
            case OPERATOR:
                if (c==StrLast(S) && CharDoubleOp(c)){
                    StrPutChar(S,c); StrPutChar(S,'\0');
                    state=END;
                    (*i)++;
                } else {
                    StrPutChar(S,'\0');
                    
                    state=END;
                }
                break;
            case WORD:
                if (c=='"') state=WORD;
                else if (c==' '){
                    /*StrPutChar(S,' ');*/
                    state=END;
                } else if (CharOp(c)) state=OPERATOR;
                else {
                    StrPutChar(S,c);
                }
                break;
            default:
                break;
        }
        if (state==END ) {
            if (StrLast(S)!='\0') StrPutChar(S,'\0');
                       
            /*printf("%d\n",*i);*/
            return S;
        }
        if (!noStep) (*i)++;
        if (state==ERROR){
            if (prevstate==QUOTE) {
                err->pres=1;
                err->err=myrealloc(err->err,50);
                err->err="Unfinished Quote\0";
            }
            i=NULL;
            return NULL;
        }
    }
    i=NULL;
    return S;
}





List* ParseBuildList(char* s, Err* err){
    List* L=NULL,*Lold=NULL;
    int i=0,iold=0; Str* S=NULL; Err* err2=ErrInit();
    i=0;
    while (i<strlen(s)){
        iold=i;
        
        S=ParseLex(s, err,&i);
         
        if (S==NULL){
            myfree(err);
            err=err2;
            return NULL;
        }
        i=i;
        
        if (L==NULL) { L=ListInit(); Lold=L;}
        else { L->next=NULL; L->next=ListInit(); L=L->next;}
        ListPutDataStr(L,S->s);
        StrFree(S);
        /*printf("%s\n",S->s);*/
    }
    myfree(err2);
    return Lold;
}

MorphemeTypes ParseGetTokenClass(char* token){
    if (token==NULL) {
        printf("FUCK");
        return TEXT;
    }
    if (strlen(token)==1 && (token[0]=='&' || token[0]=='|' || token[0]==';'))
        return OPER;
    if (strlen(token)==2 && (token[0]=='&' || token[0]=='|') && (token[0]==token[1]))
        return OPER;
    if (strlen(token)==2 && (token[0]=='>') && (token[0]==token[1]))
        return REDIRECT;
    if (strlen(token)==1 && (token[0]=='>' || token[0]=='<'))
        return REDIRECT;
    if (strlen(token)==1 && (token[0]=='('))
        return SUBSHELL_BEGIN;
    if (strlen(token)==1 && (token[0]==')'))
        return SUBSHELL_END;
    return TEXT;
}

int OperatorType(char* S){
    if (strlen(S)==1){
        if(S[0]=='&') return LINK_BACKGROUND;
        if(S[0]=='|') return LINK_PIPE;
        if(S[0]==';') return LINK_SEMICOLON;
    }
    if (strlen(S)==2){
        if (S[0]=='&' && S[1]=='&') return LINK_AND;
        if (S[0]=='|' && S[1]=='|') return LINK_OR;
    }
    return LINK_NULL;
}

void ParseMorpheme(char* S, ParseStates* state, Tree** T){
    static char* redirop=NULL;
    redirop=myrealloc(redirop,3*sizeof(char));
    Tree* newT=NULL;
    switch (*state){
        case CMD:{
            switch (ParseGetTokenClass(S)){
                case TEXT:
                    StringPut(&((*T)->cmd),S);
                    (*T)->type=LINK_COMMAND;
                    *state=ARGS;
                    break;
                case SUBSHELL_BEGIN:
                    newT=TreeInit();
                    newT->type=LINK_SUBSHELL;
                    newT->left=(*T);
                    newT->parent=(*T)->parent;
                    (*T)->parent=newT;
                    if (newT->parent!=NULL){
                        if (newT->parent->left==(*T)) newT->parent->left=newT;
                        if (newT->parent->right==(*T)) newT->parent->right=newT;
                    }
                    *state=CMD;
                    break;
                case SUBSHELL_END:
                    while((*T)!=NULL &&((*T)->type!=LINK_SUBSHELL)) *T=(*T)->parent;
                    if (*T==NULL) *state=PARSEERROR;
                    else *state=AFTERSUBSHELL;
                    break;
                default:
                    *state=PARSEERROR;
                    break;
            }
            break;
        }
        case ARGS:{
            switch (ParseGetTokenClass(S)){
                case TEXT:
                    ListAdd(&((*T)->args),S);
                    *state=ARGS;
                    break;
                case REDIRECT:
                    strcpy(redirop,S);
                    *state=REDIR;
                    break;
                case OPER:
                    newT=TreeInit();
                    newT->type=OperatorType(S);
                    newT->parent=(*T)->parent;
                    if (newT->parent!=NULL) {
                        if (newT->parent->left==(*T)) newT->parent->left=newT;
                        if (newT->parent->right==(*T)) newT->parent->right=newT;
                    }
                    (*T)->parent=newT;
                    newT->left=(*T);
                    newT->right=TreeInit();
                    newT->right->parent=newT;
                    *T=newT->right;
                    *state=CMD;
                    break;
                case SUBSHELL_END:
                    while((*T)!=NULL &&((*T)->type!=LINK_SUBSHELL)) *T=(*T)->parent;
                    if (*T==NULL) *state=PARSEERROR;
                    else *state=AFTERSUBSHELL;
                    break;
                default:
                    *state=PARSEERROR;
            }
            break;
        }
        case REDIR:{
            switch (ParseGetTokenClass(S)){
                case TEXT:
                    if (!strcmp(redirop,">")) StringPut(&((*T)->out),S);
                    if (!strcmp(redirop,">>")) StringPut(&((*T)->append),S);
                    if (!strcmp(redirop,"<")) StringPut(&((*T)->in),S);
                    *state=ARGS;
                    break;
                default:
                    *state=PARSEERROR;
            }
            break;
        }
        case AFTERSUBSHELL:{
            switch (ParseGetTokenClass(S)){
                case OPER:
                    newT=TreeInit();
                    newT->type=OperatorType(S);
                    newT->parent=(*T)->parent;
                    if (newT->parent!=NULL) {
                        if (newT->parent->left==(*T)) newT->parent->left=newT;
                        if (newT->parent->right==(*T)) newT->parent->right=newT;
                    }
                    (*T)->parent=newT;
                    newT->left=(*T);
                    newT->right=TreeInit();
                    newT->right->parent=newT;
                    *T=newT->right;
                    *state=CMD;
                    break;
                case REDIRECT:
                    *state=REDIR;
                    break;
                default:
                    *state=PARSEERROR;
            }
            break;
        }
        default:
            *state=PARSEERROR;
    }
}

void ParseTreeFixNULL(Tree* T){
    if (T==NULL) return;
    if (T->type==LINK_NULL) {
        TreeFreeNode(T);
        if (T->parent!=NULL) {
            if (T->parent->left==T) T->parent->left=NULL;
            if (T->parent->right==T) T->parent->right=NULL;
        }
    }
    ParseTreeFixNULL(T->left);
    ParseTreeFixNULL(T->right);
}

Tree* ParseBuildTree(List* L,Err* E){
    if (L==NULL) return NULL;
    List* Lold;
    Tree* Told;
    Tree* T=TreeInit();
    ParseStates state=CMD;
    ParseMorpheme("(",&state,&T);
    while (state!=PARSEERROR && L!=NULL){
        Lold=L;
        ParseMorpheme((char*)L->data,&state,&T);
        L=L->next;
    }
    ParseMorpheme(")",&state,&T);
    if (state==PARSEERROR){
        E->pres=1;
        E->err=realloc(E->err,(50+strlen(Lold->data)*sizeof(char)));
        E->err="Syntax error near: \0";
        strcpy((E->err)+strlen(E->err),(char*)Lold->data);
        return NULL;
    }
    Told=T;
    while (T->parent!=NULL) T=T->parent;
    ParseTreeFixNULL(T);
    return T;
}



