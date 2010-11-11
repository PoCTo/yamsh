#ifndef LIST_H
#define LIST_H

typedef struct List{
	char* data;
	struct List* next;
} List;

List* ListInit();

#endif
