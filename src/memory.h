#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void* myrealloc(void *ptr, size_t size);
void myfree(void* ptr);

#endif
