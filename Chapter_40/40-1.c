#define _DEFAULT_SOURCE
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <utmpx.h>
#include <string.h>

/*

	Just did what is said in book. Use ttyname() to get terminal name, then move through utmp file and compare ut_line against saved name. If match, I copy
	string to userBuffer string, and return. If no match is found, I return NULL.

	I am running this on Ubuntu on a virtual machine. To open virtual console, I used

	CTRL+ALT+F3 to open virtual console (tty3).
	cTRL-ALT-F2 to return.

	Ouput on virtual console was

		USER: tty3

	Output on terminal emulator was

		USER: N\A

*/

// Statically allocated string to store results of getlogin() clone. Set to LOGIN_NAME_MAX, which is 256 on my machine.

char userBuffer[LOGIN_NAME_MAX];

char *get_login(void);

char *get_login(void){

	struct utmpx* ut;

	// + 5 to move past intial "/dev/" in returned string.

	char *controllingTerminal = ttyname(STDIN_FILENO) + 5;

	if(controllingTerminal == NULL){

		return NULL;

	}

	setutxent();

	while((ut = getutxent()) != NULL) {

		if(strncmp(ut->ut_line, controllingTerminal, sizeof(ut->ut_line)) == 0){

			strncpy(userBuffer, ut->ut_line, sizeof(ut->ut_line));

			endutxent();

			return userBuffer;

		}

	}

	endutxent();

	return NULL;

}

int main(int argc, char *argv[]){

	printf("USER: %s\n", get_login() ? userBuffer : "N\\A");

	return 0;

}
