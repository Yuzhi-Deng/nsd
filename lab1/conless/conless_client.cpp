#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

//man 7 ip
#define CLT_INADDR INADDR_ANY 
//in ip, it is 0.0.0.0. when it is specified in bind(), the socket will be bound to all local address interface

#define CLT_PORT 0x3412
#define SRV_INADDR INADDR_LOOPBACK //127.0.0.0
#define SRV_PORT 0x1234

char recvbuf[128];
int main(int argc, char* argv[]) {
  const char *sndbuf = "I am conless client";
  
  //step1: creat a endpoint for communication 
  
  /*prototype:
      int socket(int domain, int type, int protocol)
   *param:
    - domain specifies the `Address Family`
	- type specifies the service
	- protocol is literal
   *return:
    success: the lowest-numbered file descriptor not currently open for the proccess
	fail: -1 is returned and an errno is set appropriately
   */
  int s = socket(AF_INET, SOCK_DGRAM, 0); //use the default protocal of SOCK_DGRAM,  it can also use 17(udp).

  //step2: bind a name to a socket

  /*prototype:
      int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
   *param: 
    - sockfd: socket file descriptor
	- addr: the actual structure depend on the address family spcecified.
			the structure is defined as 
				struct sockaddr{
					sa_family_t sa_family;
					char		sa_data[14];

				}
			the only purepose of this structure is to cast the structure pointer in addr in order to avoid compiler warnings
	- addrlen: the size of the structure pointed to by `addr`, in bytes
	*return:
	 success: 0 is returend
	 fail: -1 is retured and an errno is set
   */

  /*
	struct sockaddr_in {
			sa_family_t    sin_family; // address family: AF_INET 
			in_port_t      sin_port;   // *port in network byte order
		    struct in_addr sin_addr;   // internet address 
    }
	// Internet address. 
    struct in_addr { uint32_t       s_addr;     // *address in network byte order };
  */
  struct sockaddr_in cltaddr; //see man 7 ip for the detailed
  cltaddr.sin_family = AF_INET;
  //according to annotion above, port and addr need to use the network byte order because these information will payload enveloped into a packet directly.
  cltaddr.sin_port = htons(CLT_PORT); // host to network short
  cltaddr.sin_addr.s_addr = htonl(CLT_INADDR); // host to network long(this long is indicate 32 bit) 

  int e = bind(s, (struct sockaddr *)&cltaddr, sizeof(cltaddr));
 
 //step3: send and recieve the message from the server 
  struct sockaddr_in srvaddr; 
	srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(SRV_PORT);
	srvaddr.sin_addr.s_addr = htonl(SRV_INADDR);

  /*prototype:
     ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
					const struct sockaddr *dest_addr, socklen_t addrlen)
   *parm:
	- buf: the buffer of the data would send
	- len: the length of the data would send, in bytes
	- flags: see more through `man sendto` 
    *return:
    On success, return the number of bytes sent. On error, -1 is returned, and errno is set	
	*notes:
	send can be used in both of connectionless socket and connection-oriented socket. `man sendto` see more.
  */
  socklen_t len = sizeof(srvaddr);
  ssize_t sndn = sendto(s, sndbuf, 16, 0, (struct sockaddr *)&srvaddr, len); //we use the default flag here.

  /*prototype:
	  ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, struct, socklen_t *addrlen)
	*param:
	- buf: the buffer to store the message recieved
	- len: the length of the buffer specify by caller
	- flags: `man recvfrom` for details
	- src_addr: if it is not NULL, and the underlying protocol provides the source address of the message, that source address
				is placed in the buffer pointed to by `src_addr`.
	- addrlen: it it is not NULL, before the call, it need to initialize to the size of the buffer pointed to by `src_addr`,
			   upon return, addrlen is updated to contain the actual size of the source address. (The returned address is 
			   truncated if the buffer provided is too small; in this case, addrlen will return a value greater than was 
			   supplied to the call. In another word, if `new_addrlen` > `orginal_addrlen`, the returned address is truncated.) 
	*return:
	on success, return the length of the message on successful completion(if a message is too long to fit the supplied the 
	buffer, `msg > len`, excess bytes may be discarded, but this depend on the type of socket).
	on error, -1 returned and an errno is set
    notes:
	if the flag `MSG_DONTWAIT`, recvfrom is nonblocking otherwise, it is blocking by default.
	it can be used in both of connectionless socket and connection-oriented socket. 
   */

  ssize_t rcvn = recvfrom(s, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&srvaddr, &len);
  printf("%d:%s\n",rcvn, recvbuf);

  //step 4: close the file descriptor
  close(s); 
  return 0;
}
