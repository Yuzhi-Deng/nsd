#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>

#define SRV_INADDR INADDR_ANY
#define SRV_PORT 0x1234 

char buf[128];
int main() {
  int s = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in srvaddr = {
    .sin_family = AF_INET,
	.sin_port = htons(SRV_PORT),
  };
  srvaddr.sin_addr.s_addr = htonl(SRV_INADDR);

  int binderr = bind(s, (struct sockaddr *)&srvaddr, sizeof(srvaddr));

  /*prototype:
     int listen(int sockfd, int backflag)
   *param:
   - sockfd: it will be marked as a passive socket
   - backflag: The backlog argument defines the maximum length to which the queue of pending connections for sockfd may grow
   *return:
   on success, zero is returned, on error, -1 is returend and errno is set.
   */
  int lsterr = listen(s, 0); //the maximum length of pending queue is 0, which means that allow only one connection. 
  while(1) {
	memset(buf, 0, sizeof(buf));
	printf("listenning...\n");

	struct sockaddr_in cltaddr;
	socklen_t len = sizeof(cltaddr);

	/*it will creat a new socket for every incoming connction, it will build a conntion between newsock and the peer socket automically*/
    int newsock = accept(s, (struct sockaddr *)&cltaddr, &len); 
	assert(newsock >= 0);
	printf("newsock:%d\n", newsock);

    ssize_t rcvn = recv(newsock, buf, sizeof(buf), 0);
    printf("recive %d bytes:%s\n", rcvn, buf);

	ssize_t snd = send(newsock, "ACK", 3, 0);
	close(newsock);
  }
  close(s);
}
