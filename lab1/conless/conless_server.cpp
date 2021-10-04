#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/ip.h>

#define SRV_PORT 0x1234
#define SRV_INADDR INADDR_ANY

char buf[128];
int main(int argc, char *argv[]) {
  struct sockaddr_in srvaddr, cltaddr;

  int s = socket(AF_INET, SOCK_DGRAM, 0);
  srvaddr.sin_family = AF_INET;
  srvaddr.sin_port = htons(SRV_PORT);
  srvaddr.sin_addr.s_addr = htonl(SRV_INADDR);

  int e = bind(s, (struct sockaddr *)&srvaddr, sizeof(srvaddr));

  while (1) {
	memset(buf, 0, sizeof(buf));
    socklen_t len = sizeof(cltaddr);
	ssize_t rcvn = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&cltaddr, &len);
	printf("%d: %s\n", rcvn, buf);
	ssize_t sndn = sendto(s, buf, rcvn, 0, (struct sockaddr *)&cltaddr, len);
	//actually, it need to through a way to break the loop
  }
  close(s);
  return 0;
}
