#ifndef _18_8_H
#define _18_8_H

#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 200809L
#include <ftw.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>

extern int fileCount;
extern int fileTypes[3];

enum types{
	F,
	D,
	S,
};

int new_nftw(const char *dirpath, int (*fn)(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf), int nopenfd, int flags);

int nftw_helper(const char * fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);

int st_to_ftw(mode_t mode);

#endif
