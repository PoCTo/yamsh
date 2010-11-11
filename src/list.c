#include "list.h"
#include "memory.h"
#include <string.h>
#include <stdlib.h>

/* Initializes List: data,next=NULL */
List* ListInit(){
	List* a;
	a==(List*) realloc(a,sizeof(List));
	a->next=NULL;
	a->data=NULL;
	return a;
}

/* Puts string into List Node */
void ListPutData(List* L, char* data){
	L->data=(char*) myrealloc(L->data,strlen(data)*sizeof(char));
	strcpy(L->data,data);
}

/* Clears elements of List Node and Node itself */
void ListClearNode(List* L){
	free(L->data);
	free(L);
}

/* Clears List*/
void ListClear(List** Lp){
	if ((*Lp)==NULL) return;
	ListClear(&((*Lp)->next));
	ListClearNode(*Lp);
	*Lp=NULL;
}

/* Inserts Node Ln into List after given Node L*/
void ListNodeInsert(List* L,List* Ln){
	Ln->next=L->next;
	L->next=Ln;
}
