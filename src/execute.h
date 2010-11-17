#ifndef EXECUTE_H
#define	EXECUTE_H

#include "tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "str.h"
#include "memory.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "parse.h"

int ExecuteTree(Tree* T);
#endif	/* EXECUTE_H */

