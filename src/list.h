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
void ListPutDataStr(List* L, char* data);
void ListAdd(List** L,char* s);
#endif
