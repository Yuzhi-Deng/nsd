#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <unistd.h>

#define MYPORT 0x3412
#define MYADDR INADDR_ANY


int main(int argc, char **argv) {
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in myaddr;
  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons(MYPORT);
  myaddr.sin_addr.s_addr = htonl(MYADDR);
  
  assert(bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr))  == 0);

  struct sockaddr_in peeraddr;
  char buf[128];
  socklen_t len = sizeof(peeraddr);
  while(1) {
    ssize_t rcvn = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&peeraddr, &len);
	printf("%ld\n", ((uint64_t *)&buf)[0]);
	sendto(sockfd, buf, rcvn, 0, (struct sockaddr *)&peeraddr, sizeof(peeraddr));
  }
  close(sockfd);
  return 0;
}
