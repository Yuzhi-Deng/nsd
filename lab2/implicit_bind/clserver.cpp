#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#define SERVER_PORT 0x1234
#define SERVER_ADDR INADDR_ANY 

int main() {
  sockaddr_in local, remote;
  char recvbuf[100];

  int s = socket(AF_INET, SOCK_DGRAM, 0);

  local.sin_family = AF_INET;
  local.sin_port = htons(SERVER_PORT);
  local.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s, (sockaddr *)&local, sizeof(local));

  socklen_t len = sizeof(remote);

  ssize_t retval;
  printf("I'm Server at 127.0.0.0:%u\n", SERVER_PORT);

  while (1) {
    retval = recvfrom(s, recvbuf, sizeof(recvbuf), 0, (sockaddr *)&remote, &len);
	assert(retval != -1);
	recvbuf[retval] = 0;
//note: address information is stored in network byte order, which means the byte recived fisrtly is in the low address. 
//	printf("remote origin IP address = %u\n", remote.sin_addr.s_addr);
	printf("remote IP address = %s\n", inet_ntoa(remote.sin_addr)); //convets the Internet host address given in network byte order, to a string in IPv4 dotted-decimal notation.
//	printf("remote port = %u\n", remote.sin_port);
	printf("remote port = %u\n", ntohs(remote.sin_port));
	printf("recv: %s\n", recvbuf);

    retval = sendto(s, "ACK", 3, 0, (sockaddr*)&remote, len);
	assert(retval != -1);
  }
  close(s);
  return 0;
}
