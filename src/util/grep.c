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
    int v=0;
    int needlen=0,haystackn=0,num=0; char* s;
    if (argc<3 || argc>4) {
        //fprintf(stderr,"USAGE: grep [-v] haystack needlefile\n");
        return 1;
    }
    
    for (i=1; i<argc; i++){
        /*if (strlen(s>1)&&sscanf(s,"%d",&num)==1){
            nonum=0;
            for (i=1; i<strlen(s); i++) if (s[i]<'0' || s[i]>'9') nonum++;  
            if (!nonum){
                if (num<0) minusn=-num; else plusn=num;
                if (minusn && plusn) {
                    fprintf(stderr,"+n and -n not acceptable\b");
                    exit(1);
                }
            }
        }*/
        if (!strcmp(argv[i],"-v")) { v=1; num=i;} 
    }
    
    if (argc==4 && v==0) {
        //fprintf(stderr,"USAGE: grep [-v] haystack needlefile\n");
        return 1;
    } 
    
    if (num==0) {needlen=2; haystackn=1;}
    if (num==1) {needlen=3; haystackn=2;}
    if (num==2) {needlen=3; haystackn=1;}
    if (num==3) {needlen=2; haystackn=1;}
    f=fopen(argv[needlen],"r");
    if (f==NULL){
        perror(argv[needlen]);
        exit(1);
    }
    while ((s=FileReadStr(f))!=NULL){
        if (v) if (strstr(s,argv[haystackn])==NULL) printf("%s\n",s);            
        if (!v) if (strstr(s,argv[haystackn])!=NULL) printf("%s\n",s);
        free(s);
    }
    fclose(f);
    
    return 0;
}
