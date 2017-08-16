#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <mysql/mysql.h>

void doit(int sockfg);

int main()
{
    int server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    pid_t pid;

    if(server_sockfd == -1){
        printf("socket error");
        return -1;
    }

	struct sockaddr_in server_sockaddr;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(8887);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(server_sockfd, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1){
		printf("bind error");
		return -1;
	}

	if(listen(server_sockfd, 20) == -1){
		printf("listen error");
		return -1;
	}

	for(;;){
		struct sockaddr_in clnt_addr;
		socklen_t clnt_addr_size = sizeof(clnt_addr);
		int clnt_sock = accept(server_sockfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

		if(clnt_sock == -1){
			printf("accept error");
			return -1;
		}

		if((pid = fork()) == 0){
			close(server_sockfd);
			doit(clnt_sock);
			close(clnt_sock);
			exit(0);
		}

		close(clnt_sock);
	}
}

void doit(int sockfd){
		

	char str[] = "hello world";
	sleep(3);
	write(sockfd, str, sizeof(str));
}






