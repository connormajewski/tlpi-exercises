#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "name_value_pair.h"

#define PORT_NUMBER 50002
#define BUFFER_LENGTH 256
#define NAME_LENGTH 64
#define ADDRSTRLEN 64
/*

	I could store pairs in file or something but for exercise purposes I think this is probably fine.

	Going to store pairs as singly linked-list.

*/

struct pair *head = NULL;

/*

	Replacing paresCommand with a parseBuffer function that takes received buffer from recvfrom and parses tokens inside.

*/

int parseBuffer(char *buffer, char *addr, uint32_t port);

int parseBuffer(char *buffer, char *addr, uint32_t port){

	// Values to grab from buffer.

	char command;
	char * name;
	char * value;

	// Tokenize and store. Valid commmand will always have name and value. for modification, a second name and value is needed.

	char *token = strtok(buffer, " ");

	command = token[0];

	if(command == 'p') {

		printPairList(&head);

		return 0;

	}

	token = strtok(NULL, " ");

	name = token;

	token = command == 'm' ? strtok(NULL, " ") : strtok(NULL, "\n");

	value = token;

	switch(command){

		case 'a':
		case 'd':
			return command == 'a' ? addPair(&head, name, value, addr, port) : deletePair(&head, name, value, addr, port);
		case 'm':

			char *newname;
			char *newvalue;

			token = strtok(NULL, " ");

			newname = token;

			token = strtok(NULL, "\n");

			newvalue = token;

			return modifyPair(&head, name, value, newname, newvalue, addr, port);
		default:
			printf("unknown command %c\n", command);
			break;

	}

	return -1;

}

int main(int argc, char *argv[]){

	int serverFileDescriptor;

	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;

	char buffer[BUFFER_LENGTH];
	char clientAddressString[INET_ADDRSTRLEN];

	ssize_t numBytes;
	socklen_t length;

	// Create server socket.

	serverFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

	if(serverFileDescriptor == -1){

		perror("socket()");

		return -1;

	}

	printf("socket created.\n");

	// Bind socket.

	memset(&serverAddress, 0, sizeof(struct sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT_NUMBER);

	if(bind(serverFileDescriptor, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_in)) == -1){

		perror("bind()");

		return -1;

	}

	printf("socket bound.\nwaiting for client.\n");

	for(;;){

		length = sizeof(struct sockaddr_in);

		numBytes = recvfrom(serverFileDescriptor, &buffer, BUFFER_LENGTH, 0,(struct sockaddr *) &clientAddress, &length);

		if(numBytes == -1){

			perror("recvfrom()");

			return -1;

		}

		// For security I am going to add port number to pair struct.

		if(inet_ntop(AF_INET, &clientAddress.sin_addr, clientAddressString, INET_ADDRSTRLEN) == NULL){

			perror("inet_ntop()");

			return -1;

		}

		// I guess I would break down string into parts,

		int commandStatus = parseBuffer(buffer, clientAddressString, ntohs(clientAddress.sin_port));

		snprintf(buffer, sizeof(int), "%d", commandStatus);

		if(sendto(serverFileDescriptor, buffer, sizeof(int), 0, (struct sockaddr *) &clientAddress, length) != sizeof(int)){

			perror("sendto()");

			return -1;

		}

	}

	return 0;

}
