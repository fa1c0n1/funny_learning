#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
	struct stat stat_buf;
	uid_t ruid, euid, suid;
	gid_t rgid, egid, sgid;

	if (stat("test.txt", &stat_buf) == -1) {
		perror("fail to stat");
		exit(1);
	}

	printf("----------------------------\n");
	getresuid(&ruid, &euid, &suid);
	getresgid(&rgid, &egid, &sgid);
	printf("ruid=%d, euid=%d, suid=%d\n", ruid, euid, suid);
	printf("rgid=%d, egid=%d, sgid=%d\n", rgid, egid, sgid);
	printf("----------------------------\n");

	printf("before chown\n");
	printf("the owner of test.txt is: %u\n", stat_buf.st_uid);
	printf("the group of test.txt is: %u\n", stat_buf.st_gid);

	//change owner to root, don't change group
	/*
	 * 更改文件的所有者是一项需要谨慎处理的操作,因为可能会导致安全问题.
	 * 因此并不是所有的用户都可以修改文件所有者ID的。如果需要修改一个文件
	 * 的所有者ID和组ID，该用户必须是root，或者需要同时满足以下3点才可以
	 * 成功更改文件的所有者：
	 *   1) 修改文件的进程的有效用户ID等于该文件的ID，也就是说修改操作经过文件所有者的授权.
	 *   2) 参数owner为-1 或者文件的用户ID, 也就是说文件的用户可以不改或者改成原所有者
	 *      的文件. (相当于没有改变文件的所有者)
	 *   3) 参数group等于进程的有效组ID，也就是说只能将文件改所在的组中.
	 *  
	 * PS: 即使用户ID表示的用户根本不存在，chown函数也不会检查到该错误。
	 *     只有在操作该文件时，这个问题才有可能暴露出来。
	 */
	if (chown("test.txt", 0, -1) == -1) {
		perror("fail to chown");
		exit(1);
	}

	printf("change test.txt owner successfully\n");

	if (stat("test.txt", &stat_buf) == -1) {
		perror("fail to stat");
		exit(1);
	}

	printf("after chown\n");
	printf("the owner of test.txt is: %u\n", stat_buf.st_uid);
	printf("the group of test.txt is: %u\n", stat_buf.st_gid);

	return 0;
}