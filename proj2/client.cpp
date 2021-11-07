#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#define SERVER_PORT 0x1234
#define SERVER_ADDR INADDR_LOOPBACK

#define STDIN 0


int main() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  assert(sock != -1);

  sockaddr_in srvaddr;
  srvaddr.sin_family = AF_INET;
  srvaddr.sin_port = htons(SERVER_PORT);
  srvaddr.sin_addr.s_addr = htonl(SERVER_ADDR);

  assert(connect(sock, (sockaddr *)&srvaddr, sizeof(srvaddr)) == 0);

  fd_set readfds, writefds, exceptfds;
  FD_ZERO(&readfds); FD_ZERO(&writefds); FD_ZERO(&exceptfds);

  char sndbuf[128], rcvbuf[128];
  int is_ready = 0;
  int cur = 0;
  for (; ;) {
    FD_SET(sock, &readfds); FD_SET(STDIN, &readfds);
    FD_SET(sock, &writefds); FD_SET(STDIN, &writefds);
    FD_SET(sock, &exceptfds); FD_SET(STDIN, &exceptfds);

	timeval tv = {1, 0};

	int nfd = select(10, &readfds, &writefds, &exceptfds, &tv);
	assert(nfd != -1);
	if (nfd == 0) continue; 

	if (FD_ISSET(STDIN, &readfds)) {
      ssize_t rdn =read(STDIN, &sndbuf[cur], sizeof(sndbuf) - 1 - cur);	
	  cur += rdn;
	  if (sndbuf[cur - 1] == '\n') {
	    sndbuf[cur - 1] = 0;
		cur = 0;
		is_ready = 1;
	  }
	}
    
    if (FD_ISSET(sock, &readfds)) {
      ssize_t rcvn = recv(sock, rcvbuf, sizeof(rcvbuf), 0);
	  if (rcvn == 0) {
		printf("close a socket\n");
		close(sock);
		return 0;
	  } 
	  if (rcvn <= -1) {
		perror("Error");
		close(sock);
		return 0;
	  }
	  else {
	    rcvbuf[rcvn] = 0;
		printf("-->%s\n", rcvbuf);
	  }
	}

	if (FD_ISSET(sock, &writefds)) {
	  if (is_ready) {
		send(sock, sndbuf, strlen(sndbuf), 0);
		is_ready = 0;
	  }
	}

	if (FD_ISSET(sock, &exceptfds)) {
	  printf("socket %d exception", sock);
	  perror("");
	  close(sock);
	  return 0;
	}
	
  }
}
