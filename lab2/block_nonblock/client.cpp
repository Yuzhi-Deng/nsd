#include <unistd.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <assert.h>
#include <sys/types.h>
#include <string.h>

char buf[2048];

#define SERVER_ADDR INADDR_LOOPBACK
#define SERVER_PORT 0x1234

int main() {
  int s = socket(AF_INET, SOCK_STREAM, 0); 
  assert(s != -1);
  
  sockaddr_in server; 
  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);
  server.sin_addr.s_addr = htonl(SERVER_ADDR);

  assert(connect(s, (sockaddr *)&server, sizeof(server)) == 0);  
 
  for (; ;) {
    printf("Send = "); 
	scanf("%s", buf);

	if (strcmp(buf, "exit") == 0) break;

	send(s, buf, strlen(buf), 0);
    int rcvn= recv(s, buf, 100, 0);
	if (rcvn <= 0) break;
	buf[rcvn] = 0;
	printf("%s\n", buf);
  }
  close(s);
  printf("passed\n");

  return 0;
}

