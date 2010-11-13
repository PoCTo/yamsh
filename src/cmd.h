#ifndef CMD_H
#define CMD_H

#define LINK_LAST        0x1
#define LINK_BACKGROUND  0x2
#define LINK_AND         0x4
#define LINK_OR          0x8
#define LINK_PIPE        0x10
#define LINK_SEMICOLON   0x20

typedef enum {
	FALSE, 
	TRUE
} bool;

typedef struct Cmd {
    char* exec;
    struct List* params;
    char* in;
    char* out;
    struct Cmd* next;
    struct Cmd* prev;
    int linkType;
} Cmd;

Cmd* CmdInit();
void CmdClearNode(Cmd* L);
void CmdClear(Cmd** Lp);
void CmdNodeInsert(Cmd* L,Cmd* Ln);

#endif
