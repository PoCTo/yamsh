#ifndef LIST_H
#define LIST_H

typedef struct List{
	char* data;
	struct List* next;
	struct List* prev;
} List;

List* ListInit();
void ListPutData(List* L, char* data);
void ListClearNode(List* L);
void ListClear(List** Lp);
void ListNodeInsert(List* L,List* Ln);
#endif
