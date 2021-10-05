#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define CLT_PORT 0x1234
#define CLT_INADDR INADDR_ANY
#define SRV_PORT 0x3412
#define SRV_INADDR INADDR_LOOPBACK

int main() {
  char sendbuf[] = "I am client 1";
  char recvbuf[100];

  int s = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in cltaddr = {
    .sin_family = AF_INET,
    .sin_port = htons(CLT_PORT),
  };
  cltaddr.sin_addr.s_addr = htonl(CLT_INADDR);

  bind(s, (struct sockaddr *)&cltaddr, sizeof(cltaddr));

  struct sockaddr_in srvaddr = {
	.sin_family = AF_INET,
	.sin_port = htons(SRV_PORT),
  };
  srvaddr.sin_addr.s_addr = htonl(SRV_INADDR);
  int connum = 0;
  int retval;
  do {
    retval = connect(s, (struct sockaddr*)&srvaddr, sizeof(srvaddr));  
	if (retval == -1) {
	  printf("ErrNum = %d\n", errno);
	  perror("Error:");
	}
  }while (retval != 0);

  ssize_t sndn = send(s, sendbuf, strlen(sendbuf), 0);
  if (sndn == -1) {
	printf("ErrNum = %d\n", errno);
    perror("Error:");
  }

  ssize_t rcvn = recv(s, recvbuf, sizeof(recvbuf), 0);
  if (rcvn == -1)  {
    printf("ErrNum = %d\n", errno);
    perror("Error:");
  }
  close(s);
  return 0;
}
