#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PORT_NUMBER 50002
#define BUFFER_LEN 4096

/*

	I created two seperate sockets, and connected one to the server using connect.
	I then sent some data in a loop to my server every second, once from both
	the connected and unconnected sockets. On the server side, I only received data
	from the connected socket. I am assuming this means that the unconnected socket
	will just fail silently when trying to send. If I set port to be identical to
	connected one it works fine.

*/

int main(int argc, char *argv[]){

	int serverFileDescriptor;
	int unconnectedFileDescriptor;

	struct sockaddr_in serverAddress;
	struct sockaddr_in unconnectedAddress;

	char buf[BUFFER_LEN];
	char clientAddressString[INET_ADDRSTRLEN];

	ssize_t bytesRead;
	socklen_t length;

	// Arg check

	if(argc != 2 || strcmp(argv[1], "--help") == 0){

		printf("Usage: %s data\n", argv[0]);

		return -1;

	}

	// Create server socket.

	serverFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

	if(serverFileDescriptor == -1){

		perror("socket(): ");

		return -1;

	}

	printf("socket created.\n");

	memset(&serverAddress, 0, sizeof(struct sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT_NUMBER);

	// Create second socket that is not connected.

	unconnectedFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

	if(unconnectedFileDescriptor == -1){

		perror("socket(): ");

		return -1;

	}

	printf("socket created.\n");

	memset(&unconnectedAddress, 0, sizeof(struct sockaddr_in));
	unconnectedAddress.sin_family = AF_INET;
	unconnectedAddress.sin_addr.s_addr = INADDR_ANY;
	unconnectedAddress.sin_port = htons(PORT_NUMBER + 10);

	// Connect to listening server.

	if(connect(serverFileDescriptor, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_in)) == -1){

		perror("connect()");

		return -1;

	}

	printf("client connected to localhost server.\n");

	// Loop to check for messages from client.

	printf("sending data to server...\n");

	for(;;){

		length = strlen(argv[1]);

		// Send data from connected socket.

		if(sendto(serverFileDescriptor, argv[1], length, 0, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_in)) != length){

			perror("sendto(): ");

			return -1;

		}

		// Send data from unconnected socket.

		if(sendto(serverFileDescriptor, argv[1], length, 0, (struct sockaddr *) &unconnectedAddress, sizeof(struct sockaddr_in)) != length){

			perror("sendto(): ");

			return -1;

		}

		sleep(1);

	}

	return 0;

}
