#include <string.h>
#include "parse.h"
#include "memory.h"
#include "tree.h"
#include "str.h"
#include "list.h"


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

void begin_command(ParseContext *ctx, char *command_str){
  Tree* command = NULL;
  command= TreeInit();
  command->cmd=command_str;

  ctx->current_expr = ListPush(ctx->current_expr, command);
  ctx->current_command = command;
}

void add_argument(ParseContext *ctx, char *arg){
  ctx->current_command->args =
    ListPush(ctx->current_command->args, arg);
}

void add_redirection(ParseContext *ctx, char *filename){
  TreeAddRedirection(ctx->current_command, ctx->redirect_type, filename);
}

void add_operator(ParseContext *ctx, char *operator){
  ctx->current_expr = ListPush(ctx->current_expr,
      TreeNewOperator(operator));
}

void open_subshell(ParseContext *ctx){
  ctx->expr_stack = ListPush(ctx->expr_stack, ctx->current_expr);
  ctx->current_expr = NULL;
}

int close_subshell(ParseContext *ctx){
  Tree* subshell;

  if (ctx->expr_stack == NULL)
    return 0;

  subshell = TreeNewSubshell(ListReverse(ctx->current_expr));

  ctx->current_expr = ListPush((List*)ListHead(ctx->expr_stack), subshell);
  ctx->expr_stack = ListPop(ctx->expr_stack);
  ctx->current_command = subshell;
  return 1;
}

void fix_arguments_order(Tree* node){
  List* subnodes;
  if (node == NULL) return;
  switch (node->type){
    case LINK_COMMAND:
      node->args = ListReverse(node->args);
      break;
    case LINK_SUBSHELL:
      subnodes = node->expr;
      while (subnodes != NULL)
      {
        fix_arguments_order((Tree*)ListHead(subnodes));
        subnodes = subnodes->next;
      }
      break;
    default:
      break;
  }
}

void free_command_list(List* list)
{
  while (list != NULL)
  {
    TreeFree((Tree*)ListHead(list));
    list = ListPop(list);
  }
}

List* ParseBuildList(char* s, Err* err){
    List* L=NULL,*Lold=NULL;
    int i=0,iold=0; Str* S=NULL; Err* err2=ErrInit();
    i=0;
    while (i<strlen(s)){
        iold=i;
        
        S=ParseLex(s, err,&i);
         
        if (S==NULL){
            free(err);
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
    free(err2);
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

ParseStates ParseProcessCMD(ParseContext *ctx, char* token){
        switch (ParseGetTokenClass(token)){
            case SUBSHELL_BEGIN:
                open_subshell(ctx);
                return CMD;
            case SUBSHELL_END:
                if (close_subshell(ctx))
                    return REDIR;
                else
                    return PARSEERROR;
            case TEXT:
                begin_command(ctx, token);
                return ARGS;
            default:
                return PARSEERROR;
    }
}

ParseStates ParseProcessREDIR_FILE(ParseContext *ctx, char *token){
  switch (ParseGetTokenClass(token)){
    case TEXT:
      add_redirection(ctx, token);
      return REDIR;

    default:
      return PARSEERROR;
  }
}

ParseStates ParseProcessREDIR(ParseContext *ctx, char *token){
  switch (ParseGetTokenClass(token)){
    case REDIRECT:
      ctx->redirect_type = token;
      return REDIR_FILE;

    case OPER:
      add_operator(ctx, token);
      return CMD;

    case SUBSHELL_END:
      if (close_subshell(ctx))
        return REDIR;
      else
        return PARSEERROR;

    default:
      return PARSEERROR;
  }
}

ParseStates ParseProcessARGS(ParseContext *ctx, char *token)
{
  if (ParseGetTokenClass(token) == TEXT){
    /* argument */
    add_argument(ctx, token);
    return ARGS;
  }

  /* not argument, do like REDIR */
  return ParseProcessREDIR(ctx, token);
}

 ParseStates ParseProcessToken(ParseContext* ctx, ParseStates state, char* token){
        if (state==CMD) return ParseProcessCMD(ctx,token);
        if (state==REDIR) return ParseProcessREDIR(ctx,token);
        if (state==REDIR_FILE) return ParseProcessREDIR_FILE(ctx,token);
        if (state==ARGS) return ParseProcessARGS(ctx,token);
        //No handler for PARSEERROR, as of terminal state
        return 0;
}

Tree* ParseBuildTree(List* tokens, Err* err){
    ParseContext ctx = {NULL, NULL, NULL, NULL};
    ParseStates state = CMD;

    /* Open top-level subshell */
    state = ParseProcessToken(&ctx, state, "(");
    while (tokens != NULL && state != PARSEERROR){
        state = ParseProcessToken(&ctx, state, ListHeadStr(tokens));
        tokens = tokens->next;
    }
    /* Close top-level subshell */
    if (state != PARSEERROR)
      state = ParseProcessToken(&ctx, state, ")");

    /* Check result status */
    if (state != PARSEERROR && state != REDIR_FILE &&
      ctx.expr_stack == NULL &&
      ctx.current_expr != NULL && ctx.current_expr->next==NULL){
        /* success */
        ListClear(ctx.current_expr);
        fix_arguments_order(ctx.current_command);

        err->pres = 0;
        return ctx.current_command;
    }
    else {
        /* fail */
        List* stack;
        /* free all CommandNode entities */
        free_command_list(ctx.current_expr);
        stack = ctx.expr_stack;
        while (stack != NULL){
            free_command_list(ListHead(stack));
            stack = ListPop(stack);
        }

        err->pres = 1;
        err->err=myrealloc(err->err,50*sizeof(char));
        err->err = "Syntax error\0";
        return NULL;
    }
}
   

