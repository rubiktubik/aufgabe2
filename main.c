#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>	/* for memset() */
#include <unistd.h>	/* for fork() */
#include <stdlib.h>	/* for exit() */

#include "sock_util.h"

void str_handle_http(int);
void send_good_header(int);

#define LISTENQ 5
#define SERV_PORT 5679

int main()
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t len;
    struct sockaddr_in srvaddr, cliaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&srvaddr, 0, sizeof (srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr.sin_port = htons(SERV_PORT);

    if (bind(listenfd, (struct sockaddr *) &srvaddr,
             sizeof (srvaddr)) < 0)
    {
        err_abort("Error: Cannot bind address");
    }

    listen(listenfd, LISTENQ);
    for (;;)
    {
        len = sizeof (cliaddr);
        connfd = accept(listenfd,
                        (struct sockaddr *) &cliaddr, &len);
        if (connfd < 0) err_abort("Error: accept incoming connection");
        if ((childpid = fork()) == 0)
        {
            /* child process */

            printf("New connection from %s, Port %i\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
            close(listenfd);
            //str_echo(connfd);	/* handle client */
            str_handle_http(connfd); //Bearbeite HTTP-Abfrage
            exit(0);
        }
        /* parent process */
        close(connfd);
    }
}
#define MAXLINE 128

void str_handle_http(int sockfd)
{
    ssize_t n;
    char line[MAXLINE];
    int test= 1;

    for (;;)
    {
        /* lese naechste Zeile */
        n = readline(sockfd, line, MAXLINE);
        if (n > 0)
        {
            int anzahl = (int)n;
            printf("%i Bytes empfangen!\n", anzahl);
        }

       
        if ((strcmp("GET /index.html HTTP/1.0\r\n", line) == 0) || (strcmp("GET /index.html HTTP/1.1\r\n", line) == 0))
        {
            printf("GET erhalten\n");
            
        }
        //		if( writen(sockfd, line2, n+6) != n+6 )
        //			err_abort("Fehler: writen()");
    }
}

void send_good_header(int sockfd)
{
    printf("Sende Header...");
}
