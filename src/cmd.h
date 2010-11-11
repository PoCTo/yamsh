#ifndef CMD_H
#define CMD_H

#define LINK_SEMICOLON   0
#define LINK_BACKGROUND  10
#define LINK_AND         20
#define LINK_OR          21
#define LINK_PIPE        31

typedef enum {
	FALSE, 
	TRUE
} bool;

typedef struct Cmd {
	char* exec; 
	struct List* params;
	struct List* in;
	struct List* out;
  struct Cmd* next;
  int linkType;
} Cmd;

#endif
