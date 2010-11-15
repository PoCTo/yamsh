#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/* Reallocates memory with error message if needed */
void* myrealloc(void *ptr, size_t size){
	if (ptr == NULL)
          return malloc(size);
       else
          return realloc(ptr, size);

	while (ptr==NULL){ 
		printf("ERROR: realloc() failed, trying again\n");
		sleep(1);
		ptr=realloc(ptr,size);
	}		
	return ptr;
}

void myfree(void* ptr){
    //FILE* file=fopen("e:\\debug","a");
    //printf("Freeing %16d\n",ptr);
    //fclose(file);
    free(ptr);
}
