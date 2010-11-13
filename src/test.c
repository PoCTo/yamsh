#include "cmd.h"
#include "parse.h"
#include <stdio.h>

#define pp printf("\"%s\" to %d \n\"%s\"\n\n",c,i,ReadMorpheme(c,&i))

void TestReadMorpheme(){
    char* c="abcde&&abd";
    int i=0;
    //pp; pp; pp;pp;pp;

    c="\"hui hui \" )\0"; i=0;
    //pp; pp;

    i=0;
    c="ls -la | (grep asd >   /dev/null -u || echo \"Hui\" ) &";
    pp; pp; pp; pp; pp; pp; pp; pp; pp; pp; pp; pp; pp; pp; pp;
}
