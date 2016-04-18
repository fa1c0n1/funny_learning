#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

int listener_d;  // 用来保存服务器的socket监听套接字
int connect_d;   // 用来保存服务器的socket连接套接字
 /**
  * P453: catch_signal()
  */
 int catch_signal(int sig, void (*handler)(int))
 {
	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	return sigaction(sig, &action, NULL);
 }

 /**
  * P478: read_in()
  *
  */
int read_in(int socket, char *buf, int len)
{
	char *s = buf;
	int slen = len;
	int c = recv(socket, s, slen, 0);
	while(c > 0 && s[c-1] != '\n') {
		s += c;
		slen -= c;
		c = recv(socket, s, slen, 0);
	}

	if(c < 0) {
		return c;
	} else if(c == 0) {
		buf[0] = '\0';
	} else {
		s[c-1] = '\0';
	}

	return len - slen;
}

/**
 * P479: 代码熟食
 *   
 *	error(), open_listener_socket(),
 *	bind_to_port(), say()
 */
void error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

int open_listener_socket()
{
	int s = socket(PF_INET, SOCK_STREAM, 0);
	if(s == -1) {
		error("Can't open socket");
	}

	return s;
}

void bind_to_port(int socket, int port)
{
	struct sockaddr_in name;
	name.sin_family = PF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	int reuse = 1;

	if(setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1) {
		error("Can't set the reuse option on the socket");
	}
	int c = bind(socket, (struct sockaddr *)&name, sizeof(name));
	if(c == -1) {
		error("Can't bind to socket");
	}
}

int say(int socket, char *s)
{
	int result = send(socket, s, strlen(s), 0);
	if(result == -1) {
		fprintf(stderr, "%s: %s\n", "和客户端通信时发生了错误", strerror(errno));
	}
	return result;
}

/*
 * 如果有人在服务器运行期间按了Ctrl-C,
 * 这个函数就会赶在程序结束前关闭套接字
 *
 */
void handle_shutdown(int sig)
{
	if(listener_d) {
		close(listener_d);
	}

	fprintf(stderr, "Bye!\n");
	exit(0);
}

int main(void)
{
	if (catch_signal(SIGINT, handle_shutdown) == -1) {
		error("Can't set the interrupt handle");
	}

	listener_d = open_listener_socket();
	bind_to_port(listener_d, 30000);
	if (listen(listener_d, 10) == -1) {
		error("Can't listen");
	}

	char buf[255];
	puts("Waiting for connection");

	struct sockaddr_storage client_addr;
	unsigned int address_size = sizeof(client_addr);

	while(1) {
		connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
		if(connect_d == -1) {
			error("Can't open connect socket");
		}

		//实现服务端可以同时与多个客户端通信
		if(!fork()) { //运行在子进程
			//子进程不需要主套接字,故关闭		
			close(listener_d);

			if(say(connect_d, "Internet Knock-Knock Protocal Server\r\nVersion 1.0\r\nKnock! Knock!\r\n>") != -1) {
				if(read_in(connect_d, buf, sizeof(buf)) < 0) {
					error("recv error");
				}

				if(!strncasecmp("quit", buf, 4)) {
					//子进程退出
					close(connect_d);
					exit(0);
				}

				while(strncasecmp("Who's there?", buf, 12)) {
					say(connect_d, "You should say 'Who\'s there?' !\r\n>");
					if(read_in(connect_d, buf, sizeof(buf)) < 0) {
						error("recv error");
					}
					if(!strncasecmp("quit", buf, 4)) {
						//子进程退出						
						close(connect_d);
						exit(0);
					}
				}


				if(say(connect_d, "Oscar\r\n>") != -1) {
					read_in(connect_d, buf, strlen(buf));
					if(!strncasecmp("quit", buf, 4)) {
						//子进程退出							
						close(connect_d);
						exit(0);
					}
					while(strncasecmp("Oscar who?", buf, 10)) {
						say(connect_d, "You should say 'Oscar who?' !\r\n>");
						if(read_in(connect_d, buf, sizeof(buf)) < 0) {
							error("recv error");
						}

						if(!strncasecmp("quit", buf, 4)) {
							//子进程退出							
							close(connect_d);
							exit(0);
						}
					} 

					say(connect_d, "Oscar silly question, you get a silly answer\r\n");
				}
			}

			//子进程通信结束后,退出程序
			close(connect_d);
			exit(0);
		}

		//父进程只负责接受连接,故关闭副套接字
		close(connect_d);
	}

	return 0;
}
