#include "tree.h"
#include "str.h"

#ifndef PARSE_H
#define	PARSE_H



typedef enum {
    CMD=0,ARGS,REDIR,REDIR_FILE,PARSEERROR
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


char* ReadMorpheme(char* s,int *i);
Tree* ParseString(char *s);
Str* ParseLex(char* s,Err* err,int* inew);
List* ParseBuildList(char* s, Err* err);

#endif

