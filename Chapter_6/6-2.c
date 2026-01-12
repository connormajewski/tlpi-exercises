#include <setjmp.h>
#include "tlpi_hdr.h"

static jmp_buf env;

int func(){

	if(setjmp(env) == 0){

		return 0;

	}

	return 1;

}

int main(int argc, char * argv[]){

	printf("%d\n", func());

	longjmp(env, 1);

	printf("%d\n", func());

	exit(EXIT_SUCCESS);

}
