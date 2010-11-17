#include <stdio.h>
#include "tree.h"
#include "list.h"
#include "memory.h"
#include "test.h"
#include "execute.h"

int main(int argc,char** argv){
    //TestTree();
    printf("%d\n",ExecuteCmd("cat COPYING -v"));
	return 0;
}
