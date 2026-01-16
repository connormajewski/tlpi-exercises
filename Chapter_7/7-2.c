#include "tlpi_hdr.h"

#define MINIMUM_ALLOCATION 16
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

	size = ALIGN(size);

	struct block * tmp = NULL;

	struct block * tailTmp = tail;

	while(tailTmp != NULL){

		if(tailTmp->free && tailTmp->size >= size){

			size_t remainder = tailTmp->size - size;

			if(remainder >= sizeof(struct block) + MINIMUM_ALLOCATION){

				struct block * remainderBlock = (struct block *)((char *)(tailTmp + 1) + size);

				remainderBlock->size = remainder - sizeof(struct block);
				remainderBlock->free = FREE;

				remainderBlock->next = tailTmp->next;
				remainderBlock->prev = tailTmp;

				if(tailTmp->next != NULL){

					tailTmp->next->prev = remainderBlock;

				}

				else{

					tail = remainderBlock;

				}

				tailTmp->next = remainderBlock;

			}

			tmp = tailTmp;

			tmp->free = NOT_FREE;
			tmp->size = size;

			isFreeBlock++;

			break;

		}

		tailTmp = tailTmp->prev;

	}

	if(!isFreeBlock){

		tmp = sbrk(sizeof(struct block) + size);

		if(tmp == (void *) -1){

			errno = ENOMEM;

			return NULL;

		}

		tmp->size = size;
		tmp->free = NOT_FREE;
		tmp->next = NULL;
		tmp->prev = tail;

		if(tail != NULL){

			tail->next = tmp;

		}

		else{

			heap = tmp;

		}

		tail = tmp;

	}

	return (void *) (tmp + 1);

}

void new_free(void * ptr){

	if(ptr == NULL) return;

	struct block * tmp = ((struct block * ) ptr - 1);

	tmp->free = FREE;

}

int main(int argc, char * argv){

	char * b = new_malloc((2 << 16) * sizeof(char));
	char * a = new_malloc((2 << 8) * sizeof(char));

	printList(tail);

	new_free(a);

	printList(tail);

	new_free(b);

	printList(tail);

	char * c = new_malloc((2 << 4) * sizeof(char));

	printList(tail);

	b = new_malloc((2 << 16) * sizeof(char));

	printList(tail);

	exit(EXIT_SUCCESS);

}
