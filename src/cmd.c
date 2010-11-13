#include "memory.h"
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "cmd.h"

/* Initializes List: data,next=NULL */
Cmd* CmdInit(){
	Cmd* a;
	a==(Cmd*) realloc(a,sizeof(Cmd));
	a->next=NULL;
	a->in=NULL;
        a->out=NULL;
        a->exec=NULL;
        a->linkType=LINK_LAST;
	return a;
}

/* Clears elements of List Node and Node itself */
void CmdClearNode(Cmd* L){
	free(L->in);
	free(L->out);
        ListClear(&(L->params));
        free(L->exec);
        free(L);
}

/* Clears List*/
void CmdClear(Cmd** Lp){
	if ((*Lp)==NULL) return;
	CmdClear(&((*Lp)->next));
	CmdClearNode(*Lp);
	*Lp=NULL;
}

/* Inserts Node Ln into List after given Node L*/
void CmdNodeInsert(Cmd* L,Cmd* Ln){
	Ln->next=L->next;
	L->next=Ln;
        Ln->prev=L;
        if (Ln->next != 0) Ln->next->prev=Ln;
}
