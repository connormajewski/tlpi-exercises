#include "18-8.h"
#include "dir_node.h"

int fileCount = 0;
int fileTypes[3] = {0};

/*
	This is a really shitty, barely working version. All I wanted to accomplish
	was being able to successfully traverse the directory tree. Used a stack to store directory information at each level. Does not handle nopenfd at all, and there are memory leaks.

	nftw() implementation:

	!. Need a way to keep track of open fds, and handling when too many open.
		a - Need some kind of stack storing fd, readdir() position, name, and level.
		I think FTW struct coudl work, since it has level and base.

	1. Call fn() on directory. Is processed befor other subdirs and files.
	2. Call opendir() at given directory.
	3. Popuate FTW struct, add to stack.
	4. Call readdir().
		a - If any file is itself a directory, repeat 2-4.
		b - If no files are directory, return.
			i - Pop current directory from stack, and resume.
	5. When stack is empty, there should be no more files or directories to process.
	6. Finish.
*/

int new_nftw(const char *dirpath, int (*fn)(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf), int nopenfd, int flags){

	struct dir_node * stack;

	char path[PATH_MAX];

	struct dirent * dir;
	struct stat sb;
	int s;

	int depth = 0;

	DIR * directory = opendir(dirpath);

	if(directory == NULL){

		perror("opendir()");

		return -1;

	}

	int fd = dirfd(directory);

	if(fd == -1){

		perror("dirfd()");

		return -1;

	}

	stack = new_dir_node(fd, (char *) dirpath, depth, directory, NULL, NULL);

	while(stack){

		errno = 0;

		dir = readdir(stack->d_pointer);

		if(dir == NULL){

			closedir(stack->d_pointer);

			if(!strcmp(stack->d_path, dirpath)){

				return 0;

			}

			free(stack->d_path);

			pop_dir_node(&stack);

			chdir(stack->d_path);

			continue;

		}

		if(!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) continue;

		snprintf(path, sizeof(path), "%s/%s", stack->d_path, dir->d_name);

		if(fstatat(fd, path, &sb, AT_SYMLINK_NOFOLLOW) == -1){

			perror("fstatat()");

			closedir(stack->d_pointer);

			return -1;

		}

		struct FTW ftwbuf;

		ftwbuf.level = stack->d_depth;
		ftwbuf.base = strlen(path) + 1;

		fn(path, &sb, st_to_ftw(sb.st_mode), &ftwbuf);

		if(S_ISDIR(sb.st_mode)){

			int tmp = openat(stack->d_fd, dir->d_name, O_RDONLY | O_DIRECTORY);

			if(tmp == -1){

				perror("openat()");

				continue;

			}

			DIR * tmp_dir = fdopendir(tmp);

			if(tmp_dir == NULL){

				close(tmp);

				perror("fdopendir()");

				return -1;

			}

			int sub_depth = stack->d_depth + 1;

			push_dir_node(&stack, tmp, path, sub_depth, tmp_dir);

			chdir(stack->d_path);

			continue;

		}

	}

	stack = NULL;

	return 0;

}

int nftw_helper(const char * fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){

	switch(typeflag){

		case FTW_F:
			fileTypes[F]++;
			fileCount++;
			break;
		case FTW_D:
			fileTypes[D]++;
			fileCount++;
			break;
		case FTW_SL:
			fileTypes[S]++;
			fileCount++;
			break;
	}

	return 0;

}

int st_to_ftw(mode_t mode){

	int typeflag = FTW_NS;

	switch(mode & S_IFMT){

		case S_IFREG:
			typeflag = FTW_F;
			break;
		case S_IFDIR:
			typeflag = FTW_D;
			break;
		case S_IFLNK:
			typeflag = FTW_SL;
			break;

	}

	return typeflag;

}

int main(int argc, char * argv[]){

	const struct stat *sb;
	int typeflag;
	struct FTW *ftwbuf;

	if(argc != 2){

		printf("Usage: ./18-8 directory.\n");

		return -1;

	}

	if(new_nftw(argv[1], nftw_helper, 20, FTW_PHYS) != -1){

		printf("NEW_NFTW\n----------------------------\nTOTAL FILES: %d\n----------------------------\n", fileCount);

		for(int i=0;i<3;i++) {

			printf("%-16s%-6d %.0f%%\n", i == 0 ? "Files" : i == 1 ? "Directories" : "Symbolic Links", fileTypes[i], ((double)fileTypes[i] / fileCount) * 100);

			fileTypes[i] = 0;

		}

	}

	fileCount = 0;

	if(nftw(argv[1], nftw_helper, 20, FTW_PHYS) != -1){

		printf("\nNFTW\n----------------------------\nTOTAL FILES: %d\n----------------------------\n", fileCount);

		for(int i=0;i<3;i++) {

			printf("%-16s%-6d %.0f%%\n", i == 0 ? "Files" : i == 1 ? "Directories" : "Symbolic Links", fileTypes[i], ((double)fileTypes[i] / fileCount) * 100);

		}

	}

	return 0;

}
