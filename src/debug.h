#ifndef DEBUG_H
#define	DEBUG_H

#include "str.h"
#include "parse.h"
#include <stdio.h>
#include "tree.h"

void printnode(Tree* T, FILE* f);
void printtree(Tree* T, FILE* file);
void printcommandtree(char* cmd);
#endif	/* DEBUG_H */

