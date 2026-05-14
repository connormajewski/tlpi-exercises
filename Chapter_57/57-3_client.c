#include "57-3_header.h"

/*

	This is just book code modified to not receive any response from server.
	Running this, you can see that intial transfers are instanteous, then slow
	down considerably once limit is reached.

*/

int main(int argc, char *argv[]){

	struct sockaddr_un serverAddress;
	struct sockaddr_un clientAddress;

	int serverFileDescriptor;
	int j;

	size_t messageLength;
	ssize_t bytesRead;

	char response[BUFFER_SIZE];

	if(argc < 2 || strcmp(argv[1], "--help") == 0){

		printf("Usage: %s msg...\n", argv[0]);

		return -1;

	}

	serverFileDescriptor = socket(AF_UNIX, SOCK_DGRAM, 0);

	if(serverFileDescriptor == -1){

		perror("socket(): ");

		return -1;

	}

	memset(&clientAddress, 0, sizeof(struct sockaddr_un));

	clientAddress.sun_family = AF_UNIX;

	snprintf(clientAddress.sun_path, sizeof(clientAddress.sun_path), "/tmp/ud_ucase_cl.%ld", (long) getpid());

	if(bind(serverFileDescriptor, (struct sockaddr *) &clientAddress, sizeof(struct sockaddr_un)) == -1){

		perror("bind(): ");

		return -1;

	}

	memset(&serverAddress, 0, sizeof(struct sockaddr_un));

	serverAddress.sun_family = AF_UNIX;

	strncpy(serverAddress.sun_path, SERVER_SOCK_PATH, sizeof(serverAddress.sun_path) - 1);

	// Send data in loop. Once queue limit hit, should slow down considerably.

	for(int i=0;;i++){

		messageLength = strlen(argv[1]);

		if(sendto(serverFileDescriptor, argv[1], messageLength, 0, (struct sockaddr *) & serverAddress, sizeof(struct sockaddr_un)) != messageLength){

			perror("sendto(): ");

		}

		// Gives visual of how fast data is being sent.

		if(!(i % 100)) printf("%d\n", i);

	}

	remove(clientAddress.sun_path);

	return 0;

}
