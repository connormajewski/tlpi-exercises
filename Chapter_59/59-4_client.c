#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define PORT_NUMBER 50002
#define BUF_SIZE 256
#define BUFFER_LENGTH 256

int main(int argc, char *argv[]){

	int serverFileDescriptor;

	struct sockaddr_in serverAddress;

	ssize_t numBytes;

	socklen_t length;

	char buffer[BUF_SIZE];

	printf("Usage: %s [a (add) m (modify) d (delete)] name value.\n", argv[0]);

	serverFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

	if(serverFileDescriptor == -1){

		perror("socket()");

		return -1;

	}

	memset(&serverAddress, 0, sizeof(struct sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT_NUMBER);

	length = sizeof(int);

	while(numBytes = read(STDIN_FILENO, buffer, BUF_SIZE) > 0){

		if(sendto(serverFileDescriptor, &buffer, strlen(buffer), 0, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_in)) == -1){

			perror("sendto()");

			return -1;

		}

		numBytes = recvfrom(serverFileDescriptor, &buffer, BUFFER_LENGTH, 0, (struct sockaddr *) &serverAddress, &length);

		if(numBytes == -1){

			perror("revfrom()");

			return -1;

		}

		printf("%s\n", buffer[0] == '0' ? "operation succeeded" : "operation failed");

	}

	return 0;

}
