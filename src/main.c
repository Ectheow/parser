#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/param.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#define LINESIZ 1024


void
system_error() {
    fprintf(stderr, "System error: %m!!!\n");
}

void
get_word(char **start, char **end)
{
    char *ptr = *start, *endptr = *end;
    
    while(isspace(*ptr++));
    ptr--;
    endptr = ptr;
    while(!isspace(*endptr++));
    --endptr;
    
    *end = endptr;
    *start = ptr;
}


/*
 * send format -- 
 * send  ADDRESS(address) STRING(message) [VALUE ...]
 */
void
do_send(char *args)
{
    struct sockaddr_in to_addr;
    int sockfd, n;
    char *ptr = args, *endptr, *strtolptr;
    long int portno;
    char buf[INET_ADDRSTRLEN];

    bzero(buf, sizeof(buf));
    bzero(&to_addr, sizeof(to_addr));
    
    to_addr.sin_family = AF_INET;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	system_error();
    
    get_word(&ptr, &endptr);
    
    memcpy(buf, ptr, (size_t)(endptr-ptr));
    if (inet_pton(AF_INET, buf, &to_addr.sin_addr.s_addr) <= 0)
	parse_error("bad address");

    bzero(buf, sizeof(buf));
    
    ptr = endptr;
    get_word(&ptr, &endptr);
    
    memcpy(buf, ptr, (size_t)(endptr-ptr));
    
    portno = strtol(buf, &strtolptr, 10);
    if (portno <= 0 || *strtolptr != '\0' || portno > 0xffff)
	parse_error("bad portno");

    to_addr.sin_port = htons((unsigned short)portno);

    ptr = endptr;
    while(isspace(*ptr++));
    while(*endptr++ != '\0');
    if ( (n = sendto(sockfd, ptr, (size_t)(endptr-ptr), 0, (struct sockaddr *)&to_addr, sizeof(to_addr))) < 0)
	system_error();

    close(sockfd);
}

	
int
main(int argc, char **argv)
{
    char buf[LINESIZ];
    
    bzero(buf, sizeof(buf));
    fprintf(stdout, "> ");
    while (fgets(buf, LINESIZ, stdin) != NULL) {

	if (strncmp(buf, "send", strlen("send")) == 0)
	    do_send(buf+strlen("send"));
	
	fprintf(stdout, "> ");
	bzero(buf, sizeof(buf));
    }
    return 0;
}
