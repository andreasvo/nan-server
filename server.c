#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

// Handler inclusion
#include "handler.c"

// This is a webserver for linux OS use
// Calls another program (handler.c) for request handling, see handle.c source code for implementation guidelines

#define LOCAL_PORT 80
#define CON_QUEUE 10

int main() {

	int sd, new_sd;
	uid_t new_uid = 1001;
	gid_t new_gid = 1001;
	struct sockaddr_in lok_adr;

	// Creating socket structure
	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (-1 == sd)
		exit(1);
	// OS reserves port when server dies
	if (-1 == setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)))
		exit(2);

	// initializing local adress
	lok_adr.sin_family	=	AF_INET;
	lok_adr.sin_port	=	htons((u_short)LOCAL_PORT);
	lok_adr.sin_addr.s_addr	=	htonl(INADDR_ANY);

	// Connecting socket and local adress		
	if (0 == bind(sd, (struct sockaddr *)&lok_adr, sizeof(lok_adr)))
		printf("Process %d is now connected to port %d.\n", getpid(), LOCAL_PORT);
	else
		exit(3);
	
	// Demonizing
	if (0 != fork())
		exit(2);
	
	// Chroot
	chroot("/home/andreas/webroot");

	setsid();
	signal(SIGHUP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

	// Port 80 fix
	setuid(new_uid);
	setgid(new_gid);

	if (0 != fork())
		exit(2);	

	// Awaiting request
	if (-1 == listen(sd, CON_QUEUE))
		exit(4);

	while(1) {
		
		// Accepting incoming request(s)
		new_sd = accept(sd, NULL, NULL);

		if(0 == fork()) {
			dup2(new_sd, 0);
			dup2(new_sd, 1);
			handleRequest();
			shutdown(new_sd, SHUT_RDWR);
			printf("\n");
		}
		close(new_sd);
	}
	return 0;
}