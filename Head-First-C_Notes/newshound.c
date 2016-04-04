#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char *feeds[] = {
        "http://www.cnn.com/rss/celebs.xml",
        "http://www.rollingstone.com/rock.xml",
        "http://eonline.com/gossip.xml"
    };

    int times = 3;
    char *phrase = argv[1];
    int i;
    for (i = 0; i < times; i++) {
        char var[255];
        sprintf(var, "RSS_FEED=%s", feeds[i]);
        char *vars[] = {var, NULL};

        // 首先，调用fork()克隆进程
        pid_t pid = fork();

        // 如果fork()返回-1, 就说明在克隆进程时出了问题
        if (pid == -1) {
            fprintf(stderr, "Can't fork process: %s\n", strerror(errno));
            return 1;
        }

        // 如果fork() 返回0，说明代码运行在子进程中
        if (!pid) {
            // 在子进程中应该调用exec()运行脚本
            if (execle("/usr/bin/python", "/usr/bin/python", "./rssgossip.py", 
                        phrase, NULL, vars)) {
                fprintf(stderr, "Can't run script: %s\n'", strerror(errno));
                return 1;
            }
        }

    }

    return 0;
}
