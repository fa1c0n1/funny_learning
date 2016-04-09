#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int main(int argc, char *argv[])
{
    const char *feeds = "http://drops.wooyun.org/category/mobile/feed";

    char *phrase = argv[1];
    char var[255];
    sprintf(var, "RSS_FEED=%s", feeds);
    char *vars[] = {var, NULL};
    FILE *f = fopen("stories.txt", "w");

    // 首先，调用fork()克隆进程
    // fork的返回值pid保存的是子进程ID号,但fork返回给父进程的pid和子进程的pid的值是不同的。
    // 如果pid=0，则说明代码运行在子进程中
    // 如果pid>0，则说明代码运行在父进程中
    pid_t pid = fork();

    // 如果fork()返回-1, 就说明在克隆进程时出了问题
    if (pid == -1) {
        error("Can't fork process");
    }

    // 如果fork() 返回0，说明代码运行在子进程中
    if (!pid) {
        if (dup2(fileno(f), 1) == -1) {
            error("Can't redirect Standard Output");
        }

        // 在子进程中应该调用exec()运行脚本
        if (execle("/usr/bin/python", "/usr/bin/python", "./rssgossip.py", 
               "-u", phrase, NULL, vars)) {
            error("Can't run script");
        }
    }

    // 下面这几句等待进程的代码为什么不接着上面的if后面的else语句块里写呢？
    // 因为上面的if(!pid)语句块是子进程才会执行的代码，然后子进程调用了execle()系统调用后，
    // 就不会往下执行后面的代码了，也就是说，下面这几句代码子进程是不会执行的，
    // 只有父进程才会执行. 
    int pid_status;
    if (waitpid(pid, &pid_status, 0) == -1) {
        error("等待子进程时发生了错误");
    }

    return 0;
}
