#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/types.h>

#define STDIN 0
int main() {
  fd_set readfds;
  char buf[128] = {0};
  int cur = 0;
  while (1) {
    FD_ZERO(&readfds); 
    FD_SET(STDIN, &readfds);
	struct timeval tv= {3, 0};
	int ret = select(STDIN + 1, &readfds, NULL, NULL, &tv);
	if (ret < 0) printf("error\n");
	else if (ret == 0)  printf("---\n");
	else {
	  if (FD_ISSET(STDIN, &readfds)) {
	    int rdn = read(STDIN, &buf[cur], 127 - cur);
	    cur += rdn;
	    if (buf[cur - 1] == '\n') {
		  buf[cur] = 0;
		  printf("%s", buf);
		  cur = 0;
		}
	  }
	}
  }
}
