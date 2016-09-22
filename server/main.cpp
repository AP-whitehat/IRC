//
//  main.cpp
//  basic irc
//
//  Created by Anant Prakash on 21/09/16.
//
//

/* A simple server in the internet domain using TCP
 The port number is passed as an argument */
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int servecli(int newsockfd);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    //char buffer[256];
    sockaddr_in serv_addr, cli_addr;
    //long n;
    if (argc < 2) {
        cerr<<"ERROR, no port provided\n";
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    bind(sockfd, (sockaddr *) &serv_addr,sizeof(serv_addr));
     //   error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while(1)
    {
        newsockfd=accept(sockfd,(sockaddr*) &cli_addr,&clilen);
        if(newsockfd<0)
            error("ERROR on accept");
        if(!fork())                         //fork a child process to accept multiple connections
        {
            close(sockfd);
            servecli(newsockfd);            //function that serves the client
            exit(0);
        }
        else
            error("ERROR on fork()");
        close(newsockfd);
    }
    /*
     newsockfd = accept(sockfd,
                       (sockaddr *) &cli_addr,
                       &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0) error("ERROR writing to socket");
    close(newsockfd);
     */
    close(sockfd);
    return 0;
}

int servercli(int newsockfd)
{
    
}
