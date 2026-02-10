#ifndef _DIR_NODE_H
#define _DIR_NODE_H

#include "18-8.h"

struct dir_node{

	int d_fd;
	char *d_path;
	int d_depth;
	DIR *d_pointer;

	struct dir_node *next;
	struct dir_node *prev;

};

struct dir_node * new_dir_node(int d_fd, char *d_path, int d_depth, DIR *d_pointer, struct dir_node *prev, struct dir_node *next);

void push_dir_node(struct dir_node ** head, int d_fd, char *d_path, int d_depth, DIR *d_pointer);

void pop_dir_node(struct dir_node ** head);

void print_dir_nodes(struct dir_node * head);

#endif
