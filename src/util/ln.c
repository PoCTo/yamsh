#include <stdio.h>
#include <string.h>
#include <unistd.h> 

int main(int argc, char **argv){
    if (argc<3 || argc>4) {
        fprintf(stderr,"USAGE: ln [-s] file linkname\n");
        return 1;
    }
    
    if (argc==3) {if (link(argv[1],argv[2])==-1){
        perror("ln");
        return 1;
    } else return 0;}
    
    if (strcmp(argv[1],"-s")){
        fprintf(stderr,"Unknown argument: %s\n",argv[1]);
        return 1;
    }
    
    if (symlink(argv[1],argv[2])==-1){
        perror("ln -s");
        return 1;
    }
    
    return 0;
}
