#include "list.h"

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

void ListAdd(List** L,char* s){
    if (*L==NULL){
        *L=ListInit();
        ListPutDataStr(*L,s);
        return;
    }
    while ((*L)->next!=NULL) (*L)=(*L)->next;
    (*L)->next=ListInit();
    (*L)->next->prev=(*L);
    ListPutDataStr((*L)->next,s);
    while ((*L)->prev!=NULL) (*L)=(*L)->prev;
}
