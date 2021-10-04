#include <netinet/ip.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#define CLT_INADDR INADDR_ANY
#define CLT_PORT 0x3413
#define SRV_INADDR INADDR_LOOPBACK
#define SRV_PORT 0x1234

char recvbuf[128];
int main() {
  char msg[] = "I love you !";
  
  int s = socket(AF_INET, SOCK_STREAM, 0);
  
  struct sockaddr_in cltaddr;
  cltaddr.sin_family = AF_INET;
  cltaddr.sin_port = htons(CLT_PORT);
  cltaddr.sin_addr.s_addr = htonl(CLT_INADDR);
  
  int e = bind(s, (struct sockaddr *)&cltaddr, sizeof(cltaddr));
  
  struct sockaddr_in srvaddr;
  srvaddr.sin_family = AF_INET;
  srvaddr.sin_port = htons(SRV_PORT);
  srvaddr.sin_addr.s_addr = htonl(SRV_INADDR);

  /*prototype:
     int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
   *return:
    on success, 0 is returned; on error, -1 is returned and an errno is set
   *notes:
    if the sockfd type is SOCK_DGRAM, then `addr` is the address to which datagrams are sent, and the only address from which datagrams are recieved.
	(in another word, we can also use `send` and `recv` after call `connect` in the case of connectionless).
    in the case of connected-orient, this call will attemp to make a connection to the socket that is bound to `addr`.	
	man connect for more detailed information.
   */
   
   int conerr = connect(s, (struct sockaddr *)&srvaddr, sizeof(srvaddr));
   assert(conerr == 0);
   
   ssize_t sndn = send(s, msg, strlen(msg), 0);
   printf("send %d bytes\n", sndn);

   ssize_t rcvn = recv(s, recvbuf, sizeof(recvbuf), 0);

   printf("reciev %d bytes: %s\n", rcvn, recvbuf);

   close(s);

   return 0;
}
