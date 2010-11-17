#include "list.h"

#ifndef TREE_H
#define	TREE_H

#define LINK_NULL        0x0
#define LINK_BACKGROUND  0x1
#define LINK_AND         0x2
#define LINK_OR          0x4
#define LINK_PIPE        0x8
#define LINK_SEMICOLON   0x10
#define LINK_SUBSHELL    0x20
#define LINK_COMMAND     0x40

typedef struct Tree{
    int type;
    char* cmd;
    char *in,*out,*append;
    List* args;
    struct Tree *parent,*left,*right;
    int insaved,outsaved;
} Tree;

Tree* TreeInit();
void TreeAddRedirection(Tree *node, char *type, char *filename);
Tree* TreeNewOperator(char *operator);
Tree* TreeNewSubshell(List* expression);
void TreeFreeNode(Tree* node);
void TreeFree(Tree* root);
void TreeSwap(Tree* T1, Tree* T2);
#endif	/* TREE_H */

