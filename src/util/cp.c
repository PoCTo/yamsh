#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PARTSIZE 10


int main(int argc, char **argv){
    int inf,outf;
    char line[PARTSIZE];
    int bytes;
    struct stat st; 
    if (argc!=3) {
        fprintf(stderr,"USAGE: cp sourcefile destfile\n");
        return 1;
    }

    if (stat(argv[1],&st)==-1){
        perror(argv[1]);
        return 1;
    }

    if ((st.st_mode & S_IFMT) == S_IFDIR){
        fprintf(stderr,"%s: is a directory\n",argv[1]);
        return 1;
    }

    if ((inf =open(argv[1],O_RDONLY))==-1){
        perror(argv[1]);
        return 1;
    }
    
    
    if ((outf =open(argv[2],O_WRONLY | O_CREAT),0666)==-1){
        perror(argv[1]);
        return 1;
    }
    
    while((bytes = read(inf, line, sizeof(line))) > 0)
        write(outf, line, bytes);
    
    close(inf); close(outf);
    
    return 0;
}
