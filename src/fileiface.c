#include "fileiface.h"

char* FileIfaceReadStr(FILE* f){
        char* res;
        char* tmp;
        int curlen;
        printf("hey");
        res=(char*)malloc(1);
        res[0]=0; 
        tmp=(char*) malloc(PARTSIZE+5);
        while((strlen(res)==0)||(res[strlen(res)-1]!='\n')){
                if (fgets(tmp,PARTSIZE,f)==NULL) {
                    tmp[0]='\0';
                    break;
                }
                curlen=strlen(res);
                res=realloc(res,strlen(res)+strlen(tmp)+2);
                strcpy(res+strlen(res),tmp);
        }
        if (tmp[0]=='\0') {
            myfree(tmp);
            myfree(res);
            return NULL;
        }
        res[strlen(res)-1]='\0';
        myfree(tmp);
        return res;
}
int FileIfaceRun(char* file){
    FILE* f=fopen(file,"r");
    char* s; int status=0;
    
    if (f<0){
        perror("Input file");
        return 1;
    }
    
    while ((s=FileIfaceReadStr(f))!=NULL){
        //printf("%s\n",s);
        status=ExecuteCmd(s);
        if (status) return status;        
    }
    return 0;
}
