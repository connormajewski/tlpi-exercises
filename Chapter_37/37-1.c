#include <syslog.h>
#include <string.h>
#include <stdio.h>

/*

	No clue if this is what book wanted. I just get the level from command line,
	and call syslog with it.

*/

int main(int argc, char *argv[]){

	// All values other than [0-9] default to 6.

	int level = 6;

	if(argc < 2 || argc > 3){

		printf("Usage: message [level].\n");

		return -1;

	}

	if(argc == 3){

		if(strlen(argv[2]) == 1){

			char l = argv[2][0];

			if(l >= '0' && l <= '7') level = l - '0';

		}

	}

	syslog(level, "%s", argv[1]);

	return 0;

}
