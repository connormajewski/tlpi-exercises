#include "57-3_header.h"

/*

	Modified book code, have a server that does not respond, simply
	receives the requests from the client. Added usleep() to make the blocking
	easier, since without it there would be no chance of blocking.

*/

int main(int argc, char *argv[]){

	struct sockaddr_un serverAddress;
	struct sockaddr_un clientAddress;

	int serverFileDescriptor;
	int j;

	ssize_t bytesRead;

	socklen_t length;

	char buf[BUFFER_SIZE];

	// Create server socket.

	serverFileDescriptor = socket(AF_UNIX, SOCK_DGRAM, 0);

	if(serverFileDescriptor == -1){

		perror("socket(): ");

		return -1;

	}

	if(remove(SERVER_SOCK_PATH) == -1 && errno != ENOENT){

		perror("remove(): ");

		return -1;

	}

	memset(&serverAddress, 0, sizeof(struct sockaddr_un));

	serverAddress.sun_family = AF_UNIX;

	strncpy(serverAddress.sun_path, SERVER_SOCK_PATH, sizeof(serverAddress.sun_path) - 1);

	if(bind(serverFileDescriptor, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr_un)) == -1){

		perror("bind(): ");

		return -1;

	}

	for(;;){

		length = sizeof(struct sockaddr_un);

		bytesRead = recvfrom(serverFileDescriptor, buf, BUFFER_SIZE, 0, (struct sockaddr *) &clientAddress, &length);

		if(bytesRead == -1){

			perror("recvfrom(): ");

			return -1;

		}

		printf("Server received %ld bytes from %s\n", (long) bytesRead, clientAddress.sun_path);

		usleep(100000);

	}

	return 0;

}
