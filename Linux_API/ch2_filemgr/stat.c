#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

/* stat结构,声明于sys/stat.h 头文件中, 也可通过man 2 stat查看 */

/*
struct stat {
	dev_t     st_dev;     // ID of device containing file
	ino_t     st_ino;     // inode number
	mode_t    st_mode;    // protection
	nlink_t   st_nlink;   // number of hard links
	uid_t     st_uid;     // user ID of owner
	gid_t     st_gid;     // group ID of owner
	dev_t     st_rdev;    // device ID (if special file)
	off_t     st_size;    // total size, in bytes
	blksize_t st_blksize; // blocksize for filesystem I/O
	blkcnt_t  st_blocks;  // number of 512B blocks allocated
	time_t    st_atime;   // time of last access
	time_t    st_mtime;   // time of last modification
	time_t    st_ctime;   // time of last status change
};
*/



int main(int argc, char **argv)
{
	struct stat stat_buf;

	if (argc > 1) {
		int i;
		for(i = 1; i < argc; ++i) {
			if(stat(argv[i], &stat_buf) == -1) {
				perror("fail to stat");
				exit(1);
			}

			if(S_ISREG(stat_buf.st_mode)) printf("regular file\n");
			else if(S_ISDIR(stat_buf.st_mode)) printf("directory\n");
			else if(S_ISCHR(stat_buf.st_mode)) printf("character device file\n");
			else if(S_ISBLK(stat_buf.st_mode)) printf("block device\n");
			else if(S_ISFIFO(stat_buf.st_mode)) printf("FIFO\n");
			else if(S_ISLNK(stat_buf.st_mode)) printf("symbolic link\n");
			else if(S_ISSOCK(stat_buf.st_mode)) printf("socket\n");
			else printf("unkown\n");
		}
	} else {
		printf("Wrong Usage!\n");
	}

	return 0;
}
