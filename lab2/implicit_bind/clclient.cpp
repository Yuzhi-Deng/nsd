#include <netinet/ip.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define SERVER_PORT 0x1234
#define SERVER_ADDR INADDR_LOOPBACK

int main() {
  sockaddr_in remote; 
  ssize_t retval;
  char recvbuf[128], sendbuf[128];

  int s = socket(AF_INET, SOCK_DGRAM, 0);

  socklen_t len = sizeof(remote);

  /*we don't bind to the socket that means left system bind implicitly*/
  /*
  sockaddr_in local;
  local.sin_family = AF_INET; local.sin_port = htons(12345); local.sin_addr.s_addr = htonl(INADDR_ANY);
  bind(s, (sockaddr *)&local, sizeof(local));
  */
  remote.sin_family = AF_INET;
  remote.sin_port = htons(SERVER_PORT);
  remote.sin_addr.s_addr = htonl(SERVER_ADDR);

  while (strcmp(sendbuf, "exit")) {
    printf("please enter the string to sedn('exit' to end):"); 
	scanf("%s", sendbuf);
	retval = sendto(s, sendbuf, strlen(sendbuf), 0, (sockaddr *)&remote, len);
	assert(retval != -1);
    retval = recvfrom(s, recvbuf, sizeof(recvbuf), 0, (sockaddr *)&remote, &len);
	assert(retval != -1);
	recvbuf[retval] = 0;
	printf("from server: %s\n", recvbuf);
  }
  close(s);
  
  return 0;
}
