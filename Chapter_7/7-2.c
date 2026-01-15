#include <stdlib.h>
#include "tlpi_hdr.h"

#define ALIGNMENT 16
#define ALIGN(x) (((x) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

enum MEMORY_STATES {

	NOT_FREE,
	FREE,

};

static struct block * heap = NULL;
static struct block * tail = NULL;

struct block{

	size_t size;
	int free;
	struct block *prev;
	struct block *next;

};

void * new_malloc(size_t size);
void new_free(void *ptr);

void printList(struct block * tail){

	struct block * tmp = tail;

	char buffer[32];

	char s[16];

	while(tmp != NULL){

		int m = snprintf(buffer, sizeof buffer, "%p", (void *)(tmp + 1));
		int n = snprintf(s, sizeof s, "%lu", tmp->size);

		write(1, buffer, m);
		write(1, "\t", 1);
		write(1, s, n);
		write(1, tmp->free == NOT_FREE ? "\tALLOCATED" : "\tFREE", tmp->free == NOT_FREE ? 10 : 5);
		write(1, "\n", 1);

		tmp = tmp->prev;

	}

	write(1, "\n", 1);

}

void * new_malloc(size_t size){

	int isFreeBlock = 0;

	struct block * tmp = sbrk(sizeof(struct block) + size);

	if(!size) size = 16;

	if(tmp == (void *) -1){

		errno = ENOMEM;

		return NULL;

	}

	struct block * tailTmp = tail;

	while(tailTmp != NULL){

		if(tailTmp->free && tailTmp->size >= size){

			tmp = tailTmp;

			tmp->free = NOT_FREE;
			tmp->size = size;

			isFreeBlock++;

			break;

		}

		tailTmp = tailTmp->prev;

	}

	if(!isFreeBlock){

		tmp->size = ALIGN(size);
		tmp->free = NOT_FREE;
		tmp->next = NULL;
		tmp->prev = tail;

	}

	if(tail != NULL){

		tail->next = tmp;

	}

	else{

		heap = tmp;

	}

	tail = tmp;

	return (void *) (tmp + 1);

}

void new_free(void * ptr){

	if(ptr == NULL) return;

	struct block * tmp = ((struct block * ) ptr - 1);

	tmp->free = FREE;

}

int main(int argc, char * argv){

	int stringSize = 8;

	char * tmp = (char *) new_malloc(stringSize * sizeof(char));
	char * tmp1 = (char * ) new_malloc(stringSize * 2 * sizeof(char));
	char * tmp2 = (char * ) new_malloc(stringSize * 2 * sizeof(char));
	char * tmp3 = (char * ) new_malloc((stringSize * 4)* sizeof(char));

	if(tmp == NULL){

		errExit("new_malloc()");

	}

	printList(tail);

	new_free(tmp);

	printList(tail);

	new_free(tmp3);

	tmp3 = (char * ) new_malloc((stringSize * 4 - 5)* sizeof(char));

	printList(tail);

	char * a = new_malloc(0);
	char * b = new_malloc(2 << 16 * sizeof(char));

	int size = 10;

	char ** tmp4 = new_malloc(size * sizeof(char *));
	for(int i=0;i<size;i++) tmp4[i] = new_malloc(size * sizeof(char));

	printList(tail);

	for(int i=0;i<size;i++) new_free(tmp4[i]);

	new_free(tmp4);

	new_free(b);
	b = new_malloc(2 << 8 * sizeof(char));

	printList(tail);

	exit(EXIT_SUCCESS);

}
