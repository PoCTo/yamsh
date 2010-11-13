#ifndef PARSE_H
#define	PARSE_H

typedef enum {
    INIT_NODE=0,
    READ_CMD,

} States;

char* ReadMorpheme(char* s,int *i);
Cmd* ParseString(char *s);

#endif

