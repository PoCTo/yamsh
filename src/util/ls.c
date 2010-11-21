#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

int R=0,l=0,g=0;
int notoptions=0;

void listfile(char* res, struct dirent* cur){
    struct stat s;
    if(stat(res,&s)==-1){
        perror(cur->d_name);
        return;
    }
    printf("%s ",cur->d_name);
    switch((s.st_mode & S_IFMT)){
        case S_IFDIR:
            if (l) printf("D ");
            break;
        case S_IFLNK:
            if (l) printf("L ");
        default:
            if (l) printf("F ");
    }
    if (l) printf("%ld",s.st_size);
    if (l) printf("b ");
    if (l) printf("%o ",s.st_mode & 0777);
    if (l) printf("%s ",(getpwuid(s.st_uid))->pw_name);
    if (l||g) printf("%s\n",(getgrgid(s.st_gid))->gr_name);
}

char* gpwd(){
    char* buf=NULL; int size=8;
    char* cwd;
    while ((cwd=getcwd(buf,size))==NULL){
        free(buf); buf=NULL;
        size*=2;
    }
    return cwd;
}

void list(char* dir,char* ress){
    DIR* D=NULL;
    char* res=NULL,*oldpwd=NULL;
    struct dirent* cur;
    struct stat s;
    notoptions++;

    if (strcmp(dir,".")){
        oldpwd=gpwd();
        if (chdir(dir)==-1){
            perror("ChangeDir");
            return;
        }
	}
    
    D=opendir(".");
    if (D==NULL){
        perror("ls");
        return;
    }

    while((cur=readdir(D))!=NULL){
        res=(char*)realloc(res,strlen(dir)+strlen(cur->d_name)+2);
        res[0]='\0';
        strcpy(res,dir);
        strcpy(res+strlen(res),"/");
        strcpy(res+strlen(res),cur->d_name);

        if(stat(cur->d_name,&s)==-1){
            perror(cur->d_name);
            return;
        } else {
            if ((s.st_mode & S_IFMT) == S_IFDIR
                    && strcmp(cur->d_name,".")&& strcmp(cur->d_name,"..")){
                if (R) chdir(cur->d_name);
                if (R) list(".",res);
                if (R) chdir("..");
            }
        
        }
        //free(res);
        //res=NULL;
    }
    closedir(D);
    if (R) printf("%s:\n",ress);
    D=opendir(".");    
    while((cur=readdir(D))!=NULL){
        res=(char*)realloc(res,strlen(dir)+strlen(cur->d_name)+2);
        res[0]='\0';
        strcpy(res,dir);
        strcpy(res+strlen(res),"/");
        strcpy(res+strlen(res),cur->d_name);
        
        if (!strcmp(cur->d_name,".")|| !strcmp(cur->d_name,"..")) continue;
        
        if(stat(cur->d_name,&s)==-1){
            perror(cur->d_name);
            return;
        } else {
            listfile(cur->d_name,cur);
        
        }
        //free(res);
        //res=NULL;
    }
    closedir(D);
    printf("\n");
    
    
    if (strcmp(dir,".")){
        if (chdir(oldpwd)==-1){
            perror("Getting back to dir");
            exit(1);
        }
        free(oldpwd);
        oldpwd=NULL;
    }
    free(res);
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
    if (s[0]!='-') list(s,s);
}

int main(int argc, char** argv){
    //R=1;
    int i;
    if (argc>1){
        for (i=1; i<argc; i++) parseargopt(argv[i]);
        for (i=1; i<argc; i++) parseargnotopt(argv[i]);
    }
    if (!notoptions){
        list(".",".");
    }
    return 0;
}
