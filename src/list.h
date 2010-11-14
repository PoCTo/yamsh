#ifndef LIST_H
#define LIST_H

typedef struct List{
	void* data;
	struct List* next;
	struct List* prev;
} List;

List* ListInit();
void ListPutData(List* L, void* data);
void ListClearNode(List* L);
void ListClear(List* Lp);
void ListNodeInsert(List* L,List* Ln);
char* ListHeadStr(List* L);
void* ListHead(List* L);
List* ListReverse(List* list);
void* ListPush(List* L, void* data);
List* ListPop(List* list);
void ListPutDataStr(List* L, char* data);
#endif
