Chapter 2

2.10.a  --> gcc who02.c utmplib.c -o who02
2.10.b  --> gcc whoami.c -o whoami
2.11    --> gcc cp01.c -o cp01
2.12
  |
  |--> 如果是在 utmplib.c 中去过滤有用的信息的话，就让 utmplib 失去通用性了，
       因为 utmplib 是当作一个共享函数库来使用的。

2.13 
    (2)、(3) --> gcc logout02.c utmplib.c -o logout02

    PS: 其实可以直接使用系统提供的logout()函数. --> gcc logout03.c -lutil -o logout03 ;
        因为 logout 功能的实现需要对 /var/run/utmp 文件进行读写，而普通用户对该文件没有写权限，所以执行程序时需要 sudo ;
        在 shell 命令行执行 logout 命令与我们实现的 logout 功能的程序还是有所区别的，因为 logout 命令是 shell 内置的命令, 可通过 type logout 命令查看,
            这里要注意，如果是 shell 内置命令，则执行 which <command> 是没有结果的。类似的 exit 命令也是如此。其实 logout 命令和 exit 命令执行的效果
            大同小异, 就是退出当前的 shell。两者的区别在于如果不是登录 shell，则退出只能使用 exit 命令。由于是内置命令，所以执行 logout 和 exit 命令
            都不需要特殊权限(如root);

2.14 (略)

2.15  --> gcc cp02.c -o cp02


Chapter 3

3.10、3.11、3.12  --> gcc ls01.c -o ls01
3.13、3.14 --> gcc cp03.c -o cp03
3.15  --> gcc ls02.c -o ls02
3.16  --> 0100 000 111 101 001   (type(4bit) special(3bit) user(3bit) group(3bit) other(3bit))
3.17  --> gcc test_readperm.c -o test_readperm
3.18  --> gcc ls03.c -o ls03
3.19  --> gcc ls04.c -o ls04
3.20  --> gcc chown01.c -o chown01
3.21  --> gcc cp04.c -o cp04
3.22  --> gcc lastdata.c utmplib.c -o lastdata


Chapter 4


