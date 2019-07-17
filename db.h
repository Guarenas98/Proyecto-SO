#include <semaphore.h>

typedef struct Node {
	char *name;
	char *value;
	sem_t WR;
	sem_t Mutex;
	int contador;
	struct Node *lchild;
	struct Node *rchild;
} Node_t;

extern Node_t *head;

void interpret_command(char *, char *, int);
