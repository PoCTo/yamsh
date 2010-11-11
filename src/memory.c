#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/* Reallocates memory with error message if needed */
void* myrealloc(void *ptr, size_t size){
	void* ptr2=realloc(ptr,size);
	while (ptr2==NULL){ 
		printf("ERROR: realloc() failed, trying again\n");
		sleep(1);
		ptr2=realloc(ptr,size);
	}		
	return ptr2;
}
