#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX    1024

int main(void)
{
	char buf[MAX];
	struct stat stat_buf;

	memset(buf, 0, MAX);
	if (lstat("l_test.txt", &stat_buf) == -1) {
		perror("fail to stat");
		exit(1);
	}

	printf("permission: %o\n", stat_buf.st_mode);  /* 打印符号链接的文件权限字 */
	printf("inode number: %d\n", stat_buf.st_ino);  /* 打印符号链接的i节点号 */
	printf("device number: %d\n", stat_buf.st_dev);  /* 打印符号链接的文件系统设备号 */
	printf("r-device number: %d\n", stat_buf.st_rdev);  /* 打印符号链接的硬件设备号 */
	printf("link: %d\n", stat_buf.st_nlink);  /* 打印符号链接的硬链接数 */
	printf("uid: %d\n", stat_buf.st_uid);  /* 打印符号链接的所有者用户ID */
	printf("gid: %d\n", stat_buf.st_gid);  /* 打印符号链接的所有者组ID */
	printf("access time: %d\n", stat_buf.st_atime);  /* 打印符号链接的最近一次的访问时间 */
	printf("motify time: %d\n", stat_buf.st_mtime);  /* 打印符号链接的最近一次修改文件内容的时间 */
	printf("change time: %d\n", stat_buf.st_ctime);  /* 打印符号链接的最近一次修改文件属性的时间 */
	printf("file size: %d\n", stat_buf.st_size);  /* 打印符号链接的文件大小 */
	printf("buf size: %d\n", stat_buf.st_blksize);  /* 打印符号链接的最适的缓冲区大小 */
	printf("block size: %d\n", stat_buf.st_blocks);  /* 打印符号链接的文件在外存上占用的盘块数 */

	//stat函数会获取软链接的目的文件
	if (stat("l_test.txt", &stat_buf) == -1) {
		perror("fail to stat");
		exit(1);
	}

	printf("\n--------------------------\n");

	printf("permission: %o\n", stat_buf.st_mode);  /* 打印目的文件权限字 */
	printf("inode number: %d\n", stat_buf.st_ino);  /* 打印目的文件的i节点号 */
	printf("device number: %d\n", stat_buf.st_dev);  /* 打印目的文件的文件系统设备号 */
	printf("r-device number: %d\n", stat_buf.st_rdev);  /* 打印目的文件的硬件设备号 */
	printf("link: %d\n", stat_buf.st_nlink);  /* 打印目的文件的硬链接数 */
	printf("uid: %d\n", stat_buf.st_uid);  /* 打印目的文件的所有者用户ID */
	printf("gid: %d\n", stat_buf.st_gid);  /* 打印目的文件的所有者组ID */
	printf("access time: %d\n", stat_buf.st_atime);  /* 打印目的文件的最近一次的访问时间 */
	printf("motify time: %d\n", stat_buf.st_mtime);  /* 打印目的文件的最近一次修改文件内容的时间 */
	printf("change time: %d\n", stat_buf.st_ctime);  /* 打印目的文件的最近一次修改文件属性的时间 */
	printf("file size: %d\n", stat_buf.st_size);  /* 打印目的文件的文件大小 */
	printf("buf size: %d\n", stat_buf.st_blksize);  /* 打印目的文件的最适的缓冲区大小 */
	printf("block size: %d\n", stat_buf.st_blocks);  /* 打印目的文件的文件在外存上占用的盘块数 */
	return 0;
}