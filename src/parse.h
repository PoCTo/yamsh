#ifndef PARSE_H
#define	PARSE_H

#include "tree.h"
#include "str.h"
#include <string.h>
#include "memory.h"
#include "tree.h"
#include "str.h"
#include "list.h"
#include <stdio.h>

typedef enum {
    CMD=0,ARGS,AFTERSUBSHELL,REDIR,PARSEERROR
} ParseStates;

typedef enum {
    NO,OPER,REDIRECT,SUBSHELL_BEGIN,SUBSHELL_END,TEXT
}MorphemeTypes;

typedef enum {
    BLANK=0, WORD, QUOTE, OPERATOR, ERROR, END
} ParseLexStates;

typedef struct ParseContext{
  List* expr_stack;
  List* current_expr;
  Tree* current_command;
  char* redirect_type;
} ParseContext;

Tree* ParseString(char *s);
Str* ParseLex(char* s,Err* err,int* inew);
List* ParseBuildList(char* s, Err* err);
Tree* ParseBuildTree(List* tokens, Err* err);
Tree* ParseFull(char* c, Err* E);
#endif

