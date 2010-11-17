#include "iface.h"

char* IfaceReadStr(){
	char* res;
	char* tmp;
	int curlen;
	res=(char*)malloc(1);
	res[0]=0;
	tmp=(char*) malloc(17);
	while(res[strlen(res)-1]!='\n'){
		fgets(tmp,PARTSIZE,stdin);
		curlen=strlen(res);
		res=realloc(res,strlen(res)+strlen(tmp)+1);
		strcpy(res+strlen(res),tmp);
	}
	res[strlen(res)-1]='\0';
	return res;
}
