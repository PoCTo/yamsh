#include "list.h"
#include "memory.h"
#include <string.h>
#include <stdlib.h>

/* Initializes List: data,next=NULL */
List* ListInit(){
	List* a=NULL;
	a=(List*)myrealloc(a,sizeof(List));
	a->next=NULL;
	a->data=NULL;
        a->prev=NULL;
	return a;
}

/* Puts string into List Node */
void ListPutDataStr(List* L, char* data){
	L->data=(char*) myrealloc(L->data,strlen(data));
	strcpy(L->data,data);
}

/* Clears elements of List Node and Node itself */
void ListClearNode(List* L){
	myfree(L->data);
	myfree(L);
}

/* Clears List*/
void ListClear(List* Lp){
	if ((Lp)==NULL) return;
	ListClear(Lp->next);
	ListClearNode(Lp);
}

/* Inserts Node Ln into List after given Node L*/
void ListNodeInsert(List* L,List* Ln){
	Ln->next=L->next;
	L->next=Ln;
        Ln->prev=L;
        if (Ln->next != 0) Ln->next->prev=Ln;
}


void* ListHead(List* L){
    return L->data;
}

char* ListHeadStr(List* L){
    return (char*) ListHead(L);
}

void* ListPush(List* L, void* data){
  List* new_head = ListInit();
  new_head->data=data;
  new_head->next = L;
  return new_head;
}

List* ListReverse(List* list){
  List* prev = NULL;
  while (list != NULL) {
    List* next = list->next;
    list->next = prev;
    prev = list;
    list = next;
  }
  return prev;
}

List* ListPop(List* list){
  List* head;
  head = list;
  list = list->next;
  //ListClearNode(head);
  return list;
}
