#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/capability.h>
#include <sys/stat.h>
#include <sys/fsuid.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>

struct stat st;

uint32_t parseFlag(char flagChar);
int parseAttributes(int fd, const char * attributes, char type);

uint32_t parseFlag(char flagChar){

	int ret = 0;

	switch(flagChar){

		case 'a':
			ret = FS_APPEND_FL;
			break;
		case 'A':
			ret = FS_NOATIME_FL;
			break;
		case 'c':
			ret = FS_COMPR_FL;
			break;
		case 'd':
			ret = FS_NODUMP_FL;
			break;
		case 'D':
			if(S_ISDIR(st.st_mode))	ret = FS_DIRSYNC_FL;
			else ret = 0;
			break;
		case 'E':
			ret = FS_EXTENT_FL;
			break;
		case 'i':
			ret = FS_IMMUTABLE_FL;
			break;
		case 'j':
			ret = FS_JOURNAL_DATA_FL;
			break;
		case 's':
			ret = FS_SECRM_FL;
			break;
		case 'S':
			ret = FS_APPEND_FL;
			break;
		case 't':
			ret = FS_NOTAIL_FL;
			break;
		case 'T':
			ret = FS_TOPDIR_FL;
			break;
		case 'u':
			ret = FS_UNRM_FL;
			break;
	}

	return ret;

}

int parseAttributes(int fd, const char * attributes, char type){

	int attrs;
	int zeroedFlag = 0;

	// Need to grab current flags so we can modify.

	if(ioctl(fd, FS_IOC_GETFLAGS, &attrs) == -1){

		printf("Error getting file attributes.\n");

		return -1;

	}

	// Move through each character in valid argument, get corresponding FS flag, and update file attributes accordingly.

	*attributes++;

	while(*attributes != '\0'){

		uint32_t attrFlag = parseFlag(*attributes);

		if(attrFlag > 0){

			if(type == '+') attrs |= attrFlag;
			else if(type == '-') attrs &= ~attrFlag;
			else {

				if(!zeroedFlag) {

					attrs ^= attrs;

					zeroedFlag++;

				}

				attrs |= attrFlag;

			}

		}

		*attributes++;

	}

	// After updating, set flags for file, then exit.

	if(ioctl(fd, FS_IOC_SETFLAGS, &attrs) == -1){

		perror("ioctl(): \n");

		return -1;

	}

	return 0;

}

int main(int argc, char * argv[]){

	char argType;

	if(argc <= 2 || (strcmp(argv[1], "--help") == 0)){

		printf("Usage: ./15-7 [mode][attributes] file.\n");

		return -1;

	}

	// Need to check if file exists.

	char * filepath = argv[argc - 1];

	if(stat(filepath, &st) == -1){

		printf("stat() error. %s could not be found.\n", filepath);

		return -1;

	}

	/* To change i-node flags, have to check if either:
		1- Process EUID = file UID. (for unprivileged process, check fsuid == euid)
		2- Process is privileged (CAP_FOWNER).
	*/

	uid_t fsuid = setfsuid(-1);

	uid_t fileUid = st.st_uid;

	cap_t capabilities;
	cap_flag_value_t capabilityValue;

	capabilities = cap_get_proc();

	if(capabilities == NULL){

		printf("cap_get_proc() error.\n");

		cap_free(capabilities);

		return -1;

	}

	if(cap_get_flag(capabilities, CAP_FOWNER, CAP_EFFECTIVE, &capabilityValue) == -1){

		printf("cap_get_flag() error.\n");

		cap_free(capabilities);

		return -1;

	}

	if((capabilityValue != CAP_SET) && (fsuid != fileUid)){

		printf("Process does not have permissions to modify i-node flags.\n");

		cap_free(capabilities);

		return -1;

	}

	cap_free(capabilities);

	int fd;

	if((fd =  open(filepath, O_RDWR)) == -1){

		printf("open() error.\n");

		return -1;

	}

	/* Go through each valid argument and process it. Can't simply group all similar arguments
	together then run, i.e. -a +b -b +a +c = -ab +bac, because order of operations matters. */

	for(int i=1;i<argc-1;i++){

		parseAttributes(fd, argv[i], argv[i][0]);

	}

	close(fd);

	return 0;

}
