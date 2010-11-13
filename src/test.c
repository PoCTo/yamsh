#include "cmd.h"
#include "parse.h"
#include <stdio.h>

#define trm printf("\"%s\" to %d \n\"%s\"\n\n",c,i,ReadMorpheme(c,&i))

void TestReadMorpheme(){
    char* c="abcde&&abd";
    int i=0;
    trm; trm; trm;trm;trm;

    c="\"hui hui \" )\0"; i=0;
    trm; trm;

    i=0;
    c="ls -la | (grep asd >   /dev/null -u || echo \"Hernya\" ) &";
    trm; trm; trm; trm; trm; trm; trm; trm; trm; trm; trm; trm; trm; trm; trm;
}
