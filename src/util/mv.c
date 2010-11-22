#include <stdio.h>

int main(int argc, char **argv){
    if (argc!=3) {
        fprintf(stderr,"USAGE: mv sourcefile destfile\n");
        return 1;
    }
    
    if (rename(argv[1],argv[2])==-1){
        perror("mv");
        return 1;
    }
    
    return 0;
}
