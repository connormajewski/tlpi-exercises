#include <dirent.h>
#include <pwd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

struct treenode * nodes[2 << 14] = {0};

struct treenode{

	char name[64];
	pid_t pid;
	int children_size;
	struct treenode ** children;

};

struct treenode * create_node(pid_t pid, const char * name){

	struct treenode * node = malloc(sizeof(struct treenode));

	node->pid = pid;

	strncpy(node->name, name, 64);

	node->children = NULL;

	node->children_size = 0;

	return node;

}

void add_child(struct treenode * parent, struct treenode * child){

	parent->children  = realloc(parent->children, sizeof(struct treenode) * (parent->children_size + 1));

	parent->children[parent->children_size++] = child;

}

void freeProcessTree(struct treenode * head){

	if(!head) return;

	for(int i=0;i<head->children_size;i++){

		freeProcessTree(head->children[i]);

	}

	free(head->children);

	free(head);

}

void printProcessTree(struct treenode * head, int indent, int * hasSibling){

	if(!head) return;

	for(int i=0;i<indent;i++){

		if(i == indent - 1){

			printf("%s─", hasSibling[i] ? "├" : "└");

		}

		else{

			printf("%s   ", hasSibling[i] ? "│" : " ");

		}

	}

	printf("%s(%d)\n", head->name,  head->pid);

	for(int i=0;i< head->children_size;i++){

		hasSibling[indent] = (i != head->children_size - 1);

		printProcessTree(head->children[i], indent + 1, hasSibling);

	}

}

int processParentTree(){

	uid_t userId;

	struct dirent *dir;

	char path[1024];

	char * parentDir = "/proc";

	if(userId == -1){

		printf("Error userIdFromName().\n");

		return -1;

	}

	DIR *ds = NULL;

	ds = opendir(parentDir);

	if(ds == NULL){

		printf("Error opendir().\n");

		return -1;

	}

	errno = 0;

	while((dir = readdir(ds)) != NULL){

		long conversion = strtol(dir->d_name, NULL, 10);

		if(conversion > 0){

			snprintf(path, sizeof(path), "%s/%s/status", parentDir, dir->d_name);

			int fd = open(path, O_RDONLY);

			if(fd == -1){

				if(errno = ENOENT){

					continue;

				}

				else{

					printf("Error open().\n");

					return -1;

				}

			}

			char line[256];

			FILE *fp = fdopen(fd, "r");

			int isNameFound = 0;

			char statusName[256];
			int statusPid;
			int statusPPid;

			while(fgets(line, sizeof(line), fp)){

				char * colon = strchr(line, ':');
				if(!colon) continue;

				*colon = '\0';

				char * key = line;
				char * value = colon + 1;

				if(strcmp(key, "Name") == 0){

					while(*value == ' ' || *value == '\t') {

						value++;

					}

					size_t length = strlen(value);
					if(length > 0 && value[length-1] == '\n') value[length-1] = '\0';

					strcpy(statusName, value);
				}

				if(strcmp(key, "Pid") == 0){

					sscanf(value, " %d", &statusPid);

				}

				if(strcmp(key, "PPid") == 0){

					sscanf(value, " %d", &statusPPid);

					break;

				}


			}

			struct treenode * node = nodes[statusPid];
			struct treenode * parentNode = nodes[statusPPid];

			if(!node){

				node = create_node(statusPid, statusName);

				nodes[statusPid] = node;

			}

			if(!parentNode){

				parentNode = create_node(statusPPid, "???");

				nodes[statusPPid] = parentNode;

			}

			add_child(parentNode, node);

			fclose(fp);

		}


	}

	if(errno != 0){

		printf("readdir() error.\n");

		return -1;

	}

	int t = closedir(ds);

	if(t == -1){

		printf("Error closedir().\n");

		return -1;

	}

	return 0;

}

int main(int argc, char * argv[]){

	int a = processParentTree();

	int siblings[2 << 8] = {0};

	printProcessTree(nodes[1], 0, siblings);

	freeProcessTree(nodes[1]);

	return 0;

}

