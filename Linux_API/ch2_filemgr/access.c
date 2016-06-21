#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(void)
{
	int fd;
	uid_t ruid, euid, suid;
	struct stat stat_buf;

	if(stat("test.txt", &stat_buf) == -1) {
		perror("fail to stat");
		exit(1);
	}
	printf("The file owner is %u\n", stat_buf.st_uid);

	if(getresuid(&ruid, &euid, &suid) == -1) {
		perror("fail to getresuid");
		exit(1);
	}
	printf("The read uid is %u, effective uid is %u, saved uid is %u\n", 
		(unsigned int)ruid, (unsigned int)euid, (unsigned int)suid);

	if(access("test.txt", R_OK) == -1) {
		perror("fail to access");
		exit(1);
	}

	printf("access successfully\n");

	//if R_OK, test open
	if((fd = open("test.txt", O_RDONLY)) == -1) {
		perror("fail to open");
		exit(1);
	}

	printf("ready to read\n");
	close(fd);

	return 0;
}