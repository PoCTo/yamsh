#include <stdio.h>
#include "tree.h"
#include "list.h"
#include "memory.h"
#include "debug.h"
#include "execute.h"
#include "iface.h"
#include "fileiface.h"

int main(int argc,char** argv){
    //TestTree();
    //printf("%d\n",ExecuteCmd("ls -la"));
    if (argc>2) {
        printf("Too many arguments provided\n");
        printf("USAGE:\nyamsh\nOR\nyamsh filename\n");
        return 1;
    }
    
    if (argc==2){
       return FileIfaceRun(/*argv[1]*/"asd.sh");
    }

    IfaceRun();
    return 0;
}
