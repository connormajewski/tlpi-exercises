#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define FIELD_MAX 64

struct pair{

	char name[FIELD_MAX];
	char value[FIELD_MAX];
	char addr[FIELD_MAX];

	uint32_t port;

	struct pair *next;

};

int addPair(struct pair **head, char *name, char* value, char *addr, uint32_t port);

int modifyPair(struct pair **head, char *name, char *value, char *newname, char *newvalue, char *addr, uint32_t port);

int deletePair(struct pair **head, char *name, char *value, char *addr, uint32_t port);

void printPairList(struct pair **head);
