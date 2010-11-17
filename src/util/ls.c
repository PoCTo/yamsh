#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

int R=0,l=0,g=0;
int notoptions=0;

void listfile(char* res, struct dirent* cur){
    struct stat s;
    char* d=NULL;
    if(stat(res,&s)==-1){
        perror(cur->d_name);
        return;
    }
    switch((s.st_mode & S_IFMT)){
        case S_IFDIR:
            if (l) printf("D ");
            break;
        case S_IFLNK:
            if (l) printf("L ");
        default:
            if (l) printf("F ");
    }
    printf("%s ",cur->d_name);
    if (l) printf("%d",s.st_size);
    if (l) printf("b ");
    if (l) printf("%o ",s.st_mode & 0777);
    if (l) printf("%s ",(getpwuid(s.st_uid))->pw_name);
    if (l||g) printf("%s\n",(getgrgid(s.st_gid))->gr_name);
}

void list(char* dir){
    DIR* D=opendir(dir);
    struct dirent* cur;
    struct stat s;
    notoptions++;

    if (strcmp(dir,"./")) printf("%s\n",dir);
    if (D==NULL){
        perror("ls");
    }

    while((cur=readdir(D))!=NULL){
        char* res=NULL;

        res=(char*)realloc(res,strlen(dir)+strlen(cur->d_name)+3);
        strcpy(res+strlen(res),dir);
        strcpy(res+strlen(res),"/");
        strcpy(res+strlen(res),cur->d_name);

        if(stat(cur->d_name,&s)==-1){
            perror(cur->d_name);
            return;
        } else {
            if ((s.st_mode & S_IFMT) == S_IFDIR
                    && strcmp(cur->d_name,".")&& strcmp(cur->d_name,"..")){
                
                list(res);
            }
        listfile(res,cur);
        }
        free(res);
        res=NULL;
    }
    closedir(D);
}

void parseargopt(char* s){
    int i=1,len;
    if ((len=strlen(s))<=0) return;
    if (s[0]!='-') return;
    for (i=1; i<len; i++){
        if (s[i]!='R' && s[i]!='l' && s[i]!='g'){
            printf("Unknown option: -%c\n",s[i]);
            exit(0);
        }
        if (s[i]=='R') R=1;
        if (s[i]=='l') l=1;
        if (s[i]=='g') g=1;
    }
}

void parseargnotopt(char* s){
    if ((strlen(s))<=0) return;
    if (s[0]!='-') list(s);
}

int main(int argc, char** argv){
    int i;
    if (argc>1){
        for (i=1; i<argc; i++) parseargopt(argv[i]);
        for (i=1; i<argc; i++) parseargnotopt(argv[i]);
    }
    if (!notoptions){
        list("./");
    }
}
