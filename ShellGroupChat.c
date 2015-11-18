#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <signal.h>

#define MAXPENDING 5    /* Maximum outstanding connection requests */

static void die(const char *message)
{
    perror(message);
    exit(1);
}


int main(int argc, char *argv[])
{
        //ignore SIGPIPE so that we don't terminate when we call
        //send() on a disconnected socket
        if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
                die("signal() failed");
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in servAddr; /* Local address */
    struct sockaddr_in clntAddr; /* Client address */
    unsigned short servPort;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */

    if (argc != 3)     /* Test for correct number of arguments */
    {
        fprintf(stderr, "Please enter correct number of arguments\n");
        exit(1);
    }

    servPort = atoi(argv[2]);  /* Second arg:  local port */

        char *filename = argv[1]; /* first arg: database */
   /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        die("socket() failed");
    /* Construct local address structure */
    memset(&servAddr, 0, sizeof(servAddr));   /* Zero out structure */
    servAddr.sin_family = AF_INET;                /* Internet address family */
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddr.sin_port = htons(servPort);      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        die("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        die("listen() failed");

    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(clntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &clntAddr,
                               &clntLen)) < 0)
            die("accept() failed");

        /* clntSock is connected to a client! */
        FILE *input = fdopen(clntSock, "r"); // gives FILE* to use in place of stdin
        
    /*
     * lookup loop
     */

    char line[1000];
    while (fgets(line, sizeof(line), input) != NULL) {
		char str[1030];
		sprintf(str, "%20s said: %1000s\n", "namefiller", line); 
		if(send(clntSock, str, strlen(str), 0) != strlen(str)){
			die("send failed");
		} 
		if(send(clntSock, "\n", strlen("\n"), 0) != strlen("\n")){
			die("send failed");
		}
    }

        // see if fgets() produced error
    if (ferror(input))
        die("input");

    /*
     * clean up and quit
     */
		fclose(input); // close underlying socket
    	printf("Handling client %s\n", inet_ntoa(clntAddr.sin_addr));

    }
    
    /* NOT REACHED */
}



