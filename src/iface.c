#include "iface.h"

char* IfaceReadStr(){
	char* res;
	char* tmp;
	int curlen;
	res=(char*)malloc(1);
	res[0]=0; 
	tmp=(char*) malloc(PARTSIZE+5);
	while((strlen(res)==0)||(res[strlen(res)-1]!='\n')){
		fgets(tmp,PARTSIZE,stdin);
		curlen=strlen(res);
		res=realloc(res,strlen(res)+strlen(tmp)+2);
		strcpy(res+strlen(res),tmp);
	}
	res[strlen(res)-1]='\0';
        myfree(tmp);
	return res;
}

void IfaceInvitation(){
    printf("yamsh>");
}

char* IfaceRead(){
    return IfaceReadStr();
}

void IfaceRun(){
    IfaceInvitation();
    char *s=IfaceReadStr();
    while (1){
        ExecuteCmd(s);
        myfree(s);
        IfaceInvitation();
        s=IfaceReadStr();
    }
}
