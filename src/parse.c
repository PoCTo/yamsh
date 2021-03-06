#include "parse.h"

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
    int len=0;
    static char op='\0';
    Str* S=NULL;
    S=StrInit();
    int noStep=0;
    char c;
    ParseLexStates prevstate,state=BLANK;
    
    if (s!=NULL) len=strlen(s); else return NULL;
    while ((*i)<len && s[*i]==' ') (*i)++;
    for (;(state!=ERROR && (*i)<=len);){
        c=s[*i];
        prevstate=state;
        switch (state){
            case BLANK:
                if (c=='"') state=QUOTE;
                else if (CharOp(c)) {
                    if (S->len==0 && CharDoubleOp(c)){
                        if ((*i)<len-1 && s[(*i)+1]==c){
                            StrPutChar(S,c); StrPutChar(S,c); 
                            StrPutChar(S,'\0');
                            (*i)+=2;
                            state=END;
                        } else {
                          StrPutChar(S,c); StrPutChar(S,'\0');
                          (*i)++;
                          state=END;
                        }
                    }
                    if (S->len==0 && CharSimpleOp(c)){
                        StrPutChar(S,c); StrPutChar(S,'\0');
                        state=END;
                        (*i)++;
                    }
                } else if (c==' ' || c=='\0') state=BLANK;
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
                if (c==op && CharDoubleOp(c)){
                    StrPutChar(S,c);StrPutChar(S,c); StrPutChar(S,'\0');
                    state=END;
                    (*i)++;
                } else {
                    StrPutChar(S,c);
                    StrPutChar(S,'\0');
                    state=END;
                }
                break;
            case WORD:
                if (c=='"') state=WORD;
                else if (c==' '){
                    /*StrPutChar(S,' ');*/
                    state=END;
                } else if (CharOp(c)) {
                    state=END;
                    op=c;
                }
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
                (err->err)=myrealloc(err->err,50);
                strcpy(err->err,"Unfinished Quote\0");
            }
            i=NULL;
            return NULL;
        }
        if ((*i)>=len){
            if (state==BLANK) return NULL;
            if (state!=BLANK) {
                if (StrLast(S)!='\0') StrPutChar(S,'\0');
                return S;
            }
        }
    }
    i=NULL;
    return S;
}





List* ParseBuildList(char* s, Err* err){
    List* L=NULL,*Lold=NULL;
    int i=0,iold=0; Str* S=NULL; Err* err2=ErrInit();
    i=0;
    while ((s!=NULL)&&(i<strlen(s))){
        iold=i;
        
        S=ParseLex(s, err,&i);
         
        if (S==NULL && i<strlen(s)){
            err->pres=err2->pres;
            err->err=err2->err;
            myfree(err2);
            return NULL;
        }
        if (S==NULL && i>=strlen(s)){
            break;
        }

        i=i;
        
        if (L==NULL) { L=ListInit(); Lold=L;}
        else { L->next=NULL; L->next=ListInit(); L=L->next;}
        ListPutDataStr(L,S->s);
        StrFree(S);
        /*printf("%s\n",S->s);*/
    }
    myfree(err2->err);
    myfree(err2);
    return Lold;
}

MorphemeTypes ParseGetTokenClass(char* token){
    if (token==NULL) {
        /*printf("FUCK");*/
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
                    if (newT->type==LINK_AND || newT->type==LINK_OR){
                        while ((*T)->parent->type!=LINK_SUBSHELL) (*T)=(*T)->parent;
                    }
                    if (newT->type==LINK_BACKGROUND || newT->type==LINK_SEMICOLON){
                        while ((*T)->parent->type!=LINK_SUBSHELL
                                && (*T)->parent->type!=LINK_AND
                                && (*T)->parent->type!=LINK_OR) (*T)=(*T)->parent;
                    }
                    if (newT->type==LINK_BACKGROUND || newT->type==LINK_SEMICOLON){
                        while ((*T)->parent->type!=LINK_SUBSHELL
                                && (*T)->parent->type!=LINK_AND
                                && (*T)->parent->type!=LINK_OR) (*T)=(*T)->parent;
                    }
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
                    if (newT->type==LINK_AND || newT->type==LINK_OR){
                        while ((*T)->parent->type!=LINK_SUBSHELL) (*T)=(*T)->parent;
                    }
                    if (newT->type==LINK_BACKGROUND || newT->type==LINK_SEMICOLON){
                        while ((*T)->parent->type!=LINK_SUBSHELL
                                && (*T)->parent->type!=LINK_AND
                                && (*T)->parent->type!=LINK_OR) (*T)=(*T)->parent;
                    }
                    if (newT->type==LINK_BACKGROUND || newT->type==LINK_SEMICOLON){
                        while ((*T)->parent->type!=LINK_SUBSHELL
                                && (*T)->parent->type!=LINK_AND
                                && (*T)->parent->type!=LINK_OR) (*T)=(*T)->parent;
                    }
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
                case SUBSHELL_END:
                    *T=(*T)->parent;
                    while((*T)!=NULL &&((*T)->type!=LINK_SUBSHELL)) *T=(*T)->parent;
                    if (*T==NULL) *state=PARSEERROR;
                    else *state=AFTERSUBSHELL;
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
    List* Lold=L;
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
        if (T!=NULL) while (T->parent!=NULL) T=T->parent;
        TreeFree(T);
        E->pres=1;
        E->err=realloc(E->err,(50+strlen(Lold->data)*sizeof(char)));
        strcpy(E->err,"Syntax error near: \0");
        strcpy((E->err)+strlen(E->err),(char*)Lold->data);
        return NULL;
    }
    Told=T;
    while (T->parent!=NULL) T=T->parent;
    ParseTreeFixNULL(T);
    return T;
}

Tree* ParseFull(char* c, Err* E){
    Tree* T;
    List* L=ParseBuildList(c,E);
    (E->pres)=0;
    if ((E->pres)==1) {
        ListClear(L);
        return NULL;
    }
    T=ParseBuildTree(L,E);
    ListClear(L);
    if ((E->pres)==1) {
        TreeFree(T);
        return NULL;
    }
    return T;
}



