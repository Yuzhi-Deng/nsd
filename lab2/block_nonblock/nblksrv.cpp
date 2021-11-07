#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <assert.h>


#define SERVER_PORT 0x1234
#define SERVER_ADDR INADDR_LOOPBACK

#define setaddr(addr, fm, port, ipaddr) \
   addr.sin_family = fm; addr.sin_port = htons(port); addr.sin_addr.s_addr = htonl(ipaddr);

int main() {
  char buf[128];   
  
  int s = socket(AF_INET, SOCK_STREAM, 0); 
  assert(s != -1);

  sockaddr_in srvaddr;

  setaddr(srvaddr, AF_INET, SERVER_PORT, SERVER_ADDR);

  assert(bind(s, (sockaddr *)&srvaddr, sizeof(srvaddr)) != -1);

  listen(s, 5);

  int arg = 1;
  ioctl(s, FIONBIO, &arg);


  sockaddr_in cltaddr;
  int newsock;
  socklen_t len = sizeof(cltaddr);
  for (; ;) {
	while ((newsock = accept(s, (sockaddr *)&cltaddr, &len)) == -1) {
	  if (errno != EWOULDBLOCK) perror("Error");
	}
	printf("accept newsock = %d", newsock);

	arg = 1;
	ioctl(newsock, FIONBIO, &arg);

	for (; ;) {
      int rcvn = recv(newsock, buf, sizeof(buf), 0);	
	  if (rcvn == -1) {
	    if (errno != EWOULDBLOCK) {
		  perror("Error");
		  break;
		}
	  }
	  else if (rcvn == 0) {
		perror("C");
		break;
	  }
	  else {
	    buf[rcvn] = 0;
        printf("->%s\n", buf);
		assert(send(newsock, "ACK by server", 13, 0) != -1);
	  }
	}
	close(newsock);
	printf("close socket\n");
  }

  close(s);

  return 0;
}
	
