typedef struct List{
	char* data;
	struct List* next;
} List;

List* ListInit();
