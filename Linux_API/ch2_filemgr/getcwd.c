#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX    1024

int main(void)
{
	char *dir;
	char buf[MAX];

	memset(buf, 0, MAX);
	dir = getcwd(buf, MAX);
	if (dir == NULL) {
		perror("fail to getcwd");
		printf("getcwd errno=%d\n", errno);
		exit(1);
	}

	printf("getcwd OK\n");
	printf("init cwd=%s\n", dir);
	printf("p_buf=%p\n", buf);
	printf("p_dir=%p\n", dir);

	if (chdir("/home/suhao/tmp") == -1) {
		perror("fail to chdir");
		exit(1);
	}

	printf("chdir OK\n");
	dir = getcwd(buf, MAX);
	if (dir == NULL) {
		perror("fail to getcwd");
		printf("getcwd errno=%d\n", errno);
		exit(1);
	}

	printf("parent:cwd=%s, p_cwd=%p\n", dir, dir);

	pid_t pid;

	/*
	 * fork():
	 *   On success, the PID of the child process is returned in the
	 *   parent, and 0 is returned in the child. On failure, -1 is returned
	 *   in the parent, no child process  is  created,  and errno is set
	 *   appropriately.
	 */
	pid = fork();
	if (pid == -1) {
		perror("fail to fork");
		exit(1);
	} else if (pid == 0) {
		//得到子进程的工作目录
		dir = getcwd(buf, MAX);
		if (dir == NULL) {
			perror("fail to getcwd");
			printf("getcwd errno=%d\n", errno);
			exit(1);
		}

		printf("child:cwd=%s, p_cwd=%p\n", dir, dir);
	} else {
		if (wait(NULL) == -1) {
			perror("fail to wait");
			exit(1);
		}
	}


	return 0;
}