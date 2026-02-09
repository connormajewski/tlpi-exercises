#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

int fileCount;
int fileTypes[3];

enum types{
	FIL,
	DIR,
	SYMLINK,
};

int nftw_helper(const char * fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);

int nftw_helper(const char * fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){

	switch(typeflag){

		case FTW_F:
			fileTypes[FIL]++;
			fileCount++;
			break;
		case FTW_D:
			fileTypes[DIR]++;
			fileCount++;
			break;
		case FTW_SL:
			fileTypes[SYMLINK]++;
			fileCount++;
			break;
	}

	return 0;

}

int main(int argc, char * argv[]){

	const struct stat *sb;
	int typeflag;
	struct FTW *ftwbuf;

	nftw(argv[1], nftw_helper, 20, FTW_PHYS);

	printf("----------------------------\nTOTAL FILES: %d\n----------------------------\n", fileCount);

	for(int i=0;i<3;i++) {

		printf("%-16s%-6d %.0f%%\n", i == 0 ? "Files" : i == 1 ? "Directories" : "Symbolic Links", fileTypes[i], ((double)fileTypes[i] / fileCount) * 100);

	}

	return 0;

}
