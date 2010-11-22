#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void count(char* file){
    int f=0;
    char c;
    int words=0,bytes=0,lines=0;
    int wordnext=1;
    
    if (strcmp(file,"-")) f=open(file,O_RDONLY); else f=0; 
    if (f>=0){
        while (read(f,&c,1)==1){
            bytes++;
            //printf("%d\n",c);
            if (c==' ' || c==13 || c==10 || c=='\t') wordnext=1;
            if (c!=' ' && c!=13 && c!=10 && c!='\t' && wordnext==1) {
                words++;
                wordnext=0;
            }
            if (c=='\n') lines++;
        }
        if (strcmp(file,"-"))
            printf("%d %d %d %s\n",lines,words,bytes,file);
        else
            printf("%d %d %d\n",lines,words,bytes);    
    } else {
        perror(file);
    }
    if (strcmp(file,"-")) close(f);
}

int main(int argc, char** argv){
    int i=1;
    
    for (i=1; i<argc;i++){
        count(argv[i]);    
    }
    if (argc==1) count("-");
    return 0;
}
