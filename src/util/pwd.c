#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    char* buf=NULL; int size=8;
    char* cwd;
    while ((cwd=getcwd(buf,size))==NULL){
        free(buf); buf=NULL;
        size*=2;
    }
    printf("%s\n",cwd);
    free(cwd);
    
    return 0;
}
