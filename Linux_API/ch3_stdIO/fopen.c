#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

/*

函数原型：
    FILE *fopen(const char *path, const char *mode);

mode:
    r      Open text file for reading.  The stream is positioned at the beginning of the file.

    r+     Open for reading and writing.  The stream is positioned at the beginning of the file.

    w      Truncate file to zero length or create text file for writing.  The stream is positioned at the beginning of the file.

    w+     Open  for  reading and writing.  The file is created if it does not exist, otherwise it is truncated.  The stream is 
           positioned at the begin‐ning of the file.

    a      Open for appending (writing at end of file).  The file is created if it does not exist.  The stream is positioned at 
           the end of the file.

    a+     Open for reading and appending (writing at end of file).  The file is created if it does not exist.  The initial file 
           position for reading is at the beginning of the file, but output is always appended to the end of the file.

  PS:
    The  mode  string  can  also  include the letter 'b' either as a last character or as a character between the characters in 
    any of the two-character strings described above.(比如: rb,rb+)  This is strictly for compatibility with C89 and has no effect; 
    the 'b' is ignored on all POSIX conforming systems, including Linux. (Other systems may treat text files and binary files 
    differently, and adding the 'b' may be a good idea if you do I/O to a binary file and expect that your program may be ported 
    to non-UNIX environments.)

*/

int main(void)
{
	FILE *fp;
	char c;

	if((fp = fopen("test", "r")) == NULL) {
		perror("fail to fopen");
		exit(1);
	}

	while((c = fgetc(fp)) != EOF) {
		printf("%c", c);
	}

	fclose(fp);

	return 0;
}