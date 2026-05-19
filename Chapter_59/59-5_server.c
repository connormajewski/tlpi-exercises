#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define PORT_NUMBER 50002
#define BUFFER_LEN 4096

/*

	This is basically a copy of the book example for datagram socket client/server. Very simple,
	just gets IP and port and data length.

*/

int main(int argc, char *argv[]){

	int serverFileDescriptor;

	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;

	char buf[BUFFER_LEN];
	char clientAddressString[INET_ADDRSTRLEN];

	ssize_t bytesRead;
	socklen_t length;

	// Create server socket.

	serverFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

	if(serverFileDescriptor == -1){

		perror("socket(): ");

		return -1;

	}

	printf("socket created.\n");

	// Bind to localhost.

	memset(&serverAddress, 0, sizeof(struct sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT_NUMBER);

	if(bind(serverFileDescriptor, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_in)) == -1){

		perror("bind(): ");

		return -1;

	}

	printf("socket bound to localhost.\n");

	// Loop to check for messages from client.

	printf("waiting for client...\n");

	for(;;){

		length = sizeof(struct sockaddr_in);

		bytesRead = recvfrom(serverFileDescriptor, &buf, BUFFER_LEN, 0, (struct sockaddr *) &clientAddress, &length);

		if(bytesRead == -1){

			perror("recvFrom(): ");

			return -1;

		}

		if(inet_ntop(AF_INET, &clientAddress.sin_addr, clientAddressString, INET_ADDRSTRLEN) == NULL){

			perror("inet_ntop(): ");

			return -1;

		}

		printf("Received %ld bytes from (%s, %u)\n", (long) bytesRead, clientAddressString, ntohs(clientAddress.sin_port));


	}

	return 0;

}
