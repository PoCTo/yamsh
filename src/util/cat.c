#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PARTSIZE 10

char* FileReadStr(FILE* f){
        char* res;
        char* tmp;
        int curlen;
        //printf("hey");
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
            free(tmp);
            free(res);
            return NULL;
        }
        res[strlen(res)-1]='\0';
        free(tmp);
        return res;
}


int main(int argc, char **argv){
    int i; FILE* f=NULL;
    int minusn=0;
    int num=0; char* s;
    if (argc==1) {
        fprintf(stderr,"USAGE: cat sourcefile1 sourcefile2 ...\n");
        return 1;
    }
    
    for (i=1; i<argc; i++){
        if (!strcmp(argv[i],"-n")){
            minusn=1;
        }
    }
    
    for (i=1; i<argc; i++){
        if (!strcmp(argv[i],"-n")) continue;
        f=fopen(argv[i],"r");
        if (f==NULL){
            perror(argv[1]);
            continue;
        }
        while ((s=FileReadStr(f))!=NULL){
            if (minusn) printf("%d   ",++num);
            printf("%s\n",s);
            free(s);
        }
        fclose(f);
    }
    
    return 0;
}
