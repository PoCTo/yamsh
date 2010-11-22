#include "iface.h"

char* IfaceReadStr(){
	char* res;
	char* tmp;
	int curlen;
	res=(char*)malloc(1);
	res[0]=0; 
	tmp=(char*) malloc(PARTSIZE+5);
	while((strlen(res)==0)||(res[strlen(res)-1]!='\n')){
		if (fgets(tmp,PARTSIZE,stdin)==NULL) {
	                tmp[0]='\0';
                        break;
		}
		curlen=strlen(res);
		res=realloc(res,strlen(res)+strlen(tmp)+2);
		strcpy(res+strlen(res),tmp);
	}
	if (tmp[0]==0) {
		myfree(tmp);
		myfree(res);
		return NULL;
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
	if (s==NULL) return;
        ExecuteCmd(s);
        myfree(s);
        IfaceInvitation();
        s=IfaceReadStr();
    }
}
