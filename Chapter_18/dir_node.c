#include "dir_node.h"

struct dir_node * new_dir_node(int d_fd, char *d_path, int d_depth, DIR *d_pointer, struct dir_node *prev, struct dir_node *next){

        struct dir_node * temp = malloc(sizeof(struct dir_node));

        if(temp == NULL){

                perror("malloc()");

                return NULL;

        }

        temp->d_fd = d_fd;
        temp->d_path = d_path;
        temp->d_depth = d_depth;
        temp->d_pointer = d_pointer;

        temp->next = next;
        temp->prev = prev;

        return temp;
}

void push_dir_node(struct dir_node ** head, int d_fd, char *d_path, int d_depth, DIR *d_pointer){

        struct dir_node * new_node = new_dir_node(d_fd, d_path, d_depth, d_pointer, NULL, NULL);

        new_node->prev = *head;
        new_node->next = NULL;

        if(*head != NULL) (*head)->next = new_node;

        *head = new_node;

}

void pop_dir_node(struct dir_node ** head){

        if(*head){

                struct dir_node * temp = *head;

                *head = temp->prev;

                if(*head) (*head)->next = NULL;

                free(temp);

        }

}

void print_dir_nodes(struct dir_node * head){

        struct dir_node * temp = head;

        while(temp){

                printf("Depth: %d\nPath: %s\nFd: %d\n", temp->d_depth, temp->d_path, temp->d_fd);

                temp = temp->prev;

        }

        printf("\n");

}

