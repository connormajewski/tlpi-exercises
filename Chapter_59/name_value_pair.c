#include "name_value_pair.h"

int addPair(struct pair **head, char *name, char *value, char *addr, uint32_t port){

	if(!strlen(name) || !strlen(value)) return -1;

	struct pair *temp = malloc(sizeof(struct pair));

	strcpy(temp->name, name);
	strcpy(temp->value, value);
	strcpy(temp->addr, addr);
	temp->port = port;

	temp->next = NULL;

	if(*head == NULL){

		*head = temp;

		return 0;

	}

	struct pair *dummy = *head;

	while(dummy->next != NULL){

		dummy = dummy->next;

	}

	dummy->next = temp;

	return 0;

}


int modifyPair(struct pair **head, char *name, char *value, char *newname, char *newvalue, char *addr, uint32_t port){

	struct pair *temp = *head;

	while(temp != NULL){

		if(!strcmp(temp->addr, addr) && (temp->port == port) && !strcmp(temp->value, value) && !strcmp(temp->name, name)){

			strncpy(temp->value, newvalue, strlen(newvalue));
			strncpy(temp->name, newname, strlen(newname));

			return 0;

		}

	}

	return -1;

}


int deletePair(struct pair **head, char *name, char *value, char *addr, uint32_t port){

	struct pair *temp = *head;
	struct pair *prev = NULL;

	while(temp != NULL){

		if(!strcmp(temp->addr, addr) && (temp->port == port) && !strcmp(temp->name, name) && !strcmp(temp->value, value)){

			if(prev == NULL) *head = temp->next;

			else prev->next = temp->next;

			free(temp);

			return 0;

		}

		prev =temp;

		temp = temp->next;

	}

	return -1;

}

void printPairList(struct pair **head){

	struct pair *temp = *head;

	while(temp != NULL){

		printf("(%s:%u)\t%s: %s\n", temp->addr, temp->port, temp->name, temp->value);

		temp = temp->next;

	}

	printf("\n");

}


