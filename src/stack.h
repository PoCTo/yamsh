#ifndef STACK_H
#define	STACK_H

#define LINK_NULL        0x1
#define LINK_BACKGROUND  0x2
#define LINK_AND         0x4
#define LINK_OR          0x8
#define LINK_PIPE        0x10
#define LINK_SEMICOLON   0x20
#define LINK_SUBSHELL    0x40


typedef struct Tree{
    int delim;
    char* cmd;
    char *in,*out;
    List args;
    struct Tree *left,*right;
} Tree;

#endif	/* STACK_H */

