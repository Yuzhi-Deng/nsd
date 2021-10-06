#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <unistd.h>

#define MYPORT 0x3412
#define MYADDR INADDR_ANY
#define CONN


int main(int argc, char **argv) {
#ifdef CONN
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
#else
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
#endif
  struct sockaddr_in myaddr;
  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons(MYPORT);
  myaddr.sin_addr.s_addr = htonl(MYADDR);
  
  assert(bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr))  == 0);

  struct sockaddr_in peeraddr;
  char buf[128];
  socklen_t len = sizeof(peeraddr);
  int num = 0;
#ifdef CONN
  assert(listen(sockfd, 0) == 0);
  while (1) {
	printf("listening ...\n");
    int s = accept(sockfd, (struct sockaddr *)&peeraddr, &len);
	for (; ;) {
      ssize_t rcvn = recv(s, buf, sizeof(buf), 0);
	  uint64_t v = ((uint64_t *)&buf)[0];
	  printf("#No.%d: %ld\n",++num, v);
	  if (v == 0) { close(s); break; }
	  send(s, buf, rcvn, 0);
	}
  }
#else
  while(1) {
    ssize_t rcvn = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&peeraddr, &len);
	printf("#No.%d: %ld\n", ++num, ((uint64_t *)&buf)[0]);
	sendto(sockfd, buf, rcvn, 0, (struct sockaddr *)&peeraddr, sizeof(peeraddr));
  }
#endif
  close(sockfd);
  return 0;
}
