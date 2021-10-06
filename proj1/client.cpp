#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <unistd.h>

typedef struct sample_point {
  union {
    uint64_t tv_ms; 
	char buf[sizeof(tv_ms)];
  }data;
}SamplePoint;

#define MAXN 1000
#define MYPORT 0x1234
#define MYADDR INADDR_ANY

uint64_t delay_arr[MAXN];

static uint16_t peerport = 0x3412;
static uint32_t peeraddr = INADDR_LOOPBACK;


typedef struct comm {
  int sockfd;
  int conless;
  socklen_t len;
  struct sockaddr_in myaddr;
  struct sockaddr_in peeraddr;
}CommObj;

CommObj commobj;
static int conless = 0;

static ssize_t snd_data(const void *buf, size_t len) {
  //printf("send %ld\n", ((uint64_t *)buf)[0]);
  return conless ? sendto(commobj.sockfd, buf, len, 0, 
						  (struct sockaddr *)&(commobj.peeraddr), sizeof(commobj.peeraddr))
	             : send(commobj.sockfd, buf, len, 0);
}

static ssize_t rcv_data(void *rcvbuf, size_t buflen) {
  commobj.len = sizeof(commobj.peeraddr);
  return conless ? recvfrom(commobj.sockfd, rcvbuf, buflen, 0, 
		                    (struct sockaddr *)&(commobj.peeraddr), &commobj.len)
		         : recv(commobj.sockfd, rcvbuf, buflen, 0);
}

void init_comm() {
  commobj.conless = conless;
  commobj.sockfd = socket(AF_INET, (conless ? SOCK_DGRAM : SOCK_STREAM), 0);

  commobj.myaddr.sin_family = AF_INET;
  commobj.myaddr.sin_port = htons(MYPORT);
  commobj.myaddr.sin_addr.s_addr = htonl(MYADDR);

  commobj.peeraddr.sin_family = AF_INET;
  commobj.peeraddr.sin_port = htons(peerport);
  commobj.peeraddr.sin_addr.s_addr = htonl(peeraddr);

  assert(bind(commobj.sockfd, (struct sockaddr *)&(commobj.myaddr), sizeof(commobj.myaddr)) == 0); 

  if (!conless) {
    assert(connect(commobj.sockfd, (struct sockaddr *)&(commobj.peeraddr), sizeof(commobj.peeraddr)) == 0);
  } 

  printf("Initialize the communication successfully!\n");
  
}

void fin_comm() {
  close(commobj.sockfd);
}

uint64_t get_time() {
  struct timeval now;
  uint64_t tv_ms;
  gettimeofday(&now, NULL);
   
  tv_ms = now.tv_sec * 1000 + (now.tv_usec + 500) / 1000;  //half adjust
  return tv_ms;
}

uint64_t cal_delay_per_cnt(int num) {
  uint64_t delay = 0, roundtime;
  SamplePoint sample;
  for (int i = 0; i < num; i++) {
    sample.data.tv_ms = get_time();
	size_t buflen = sizeof(sample.data.buf);
    assert(snd_data(sample.data.buf, buflen) >= 0);
	assert(rcv_data(sample.data.buf, buflen) == (ssize_t)buflen);
	roundtime = get_time() - sample.data.tv_ms;
    delay += roundtime; 
  }
  delay = (delay + 500) / num;
  return delay;
}

int main(int argc, char **argv) {

  /*intialize the communication*/
  init_comm();
  
  /*mearsure the network delay*/ 
  int cnt = 10, num = 30;
  uint64_t avg_delay = 0;
  for (int k = 1; k <= cnt; k++) {
    delay_arr[k] = cal_delay_per_cnt(num);
	printf("#Count %d: %ld ms\n", k, delay_arr[k]);
	avg_delay += delay_arr[k];
  }

  if (!conless) {
    uint64_t ed = 0; 
	snd_data(&ed, sizeof(ed));
  }
  avg_delay = (avg_delay + 500) / cnt;
  printf("Average Delay: %ld ms\n", avg_delay);


  /*save the data*/
  return 0;

  /*final the communication*/
  fin_comm();
}
