//
//  main.cpp
//  basic irc
//
//  Created by Anant Prakash on 21/09/16.
//
//

/* IRC server using TCP & fork .
 The port number is passed as an argument */
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

char comds[][10]={"/quit","/msg","/receive","/join","/nick"}; 
int servecli(int newsockfd,int c);
int parse(char buffer[]);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

struct data
{
    char a[256],name[100];
    
};

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno,pid;
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
    int c=0;                        //keeps track of clients
    while(c<2)
    {
        newsockfd = accept(sockfd,(sockaddr *) &cli_addr,&clilen);
        if(newsockfd<0)
            error("ERROR on accept");
        pid = fork();                                   //fork a child process to accept multiple connections
        if(pid<0)
            error("ERROR on fork()");
        if(pid==0)                         
        {
            close(sockfd);
            c=c+servecli(newsockfd,c);            //function that serves the client until termination returns 1 when successful
                                                  //c determines whether to write or read
            exit(0);
        }
        else
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

int servecli(int newsockfd,int c)
{
    static data x;
    int n,flag=1;
    char buffer[256];
    if(c==0)                                    //read from 1st client
    {
//        while(flag)
//        {
            bzero(buffer,256);
            n=read(newsockfd,buffer,255);
            if(n>=0)
                flag=parse(buffer);                              //parses the msg and decides what to do
            else
                error("ERROR reading from socket");
            if(flag)
            {
                bzero(x.name,sizeof(x.name));
                n=read(newsockfd,x.name,sizeof(x.name));
                if (n < 0) error("ERROR reading from socket");
                bzero(x.a,sizeof(x.a));
                n=read(newsockfd,x.a,sizeof(x.a));
                if (n < 0) error("ERROR reading from socket");
                close(newsockfd);
                return 1;
            }
            else
                close(newsockfd);
//        }
    }
    else if (c==1)                                              //writes the msg to second client
    {
        n=write(newsockfd,x.name,sizeof(x.name));
        if (n < 0) error("ERROR writing to socket");
        n=write(newsockfd,x.a,sizeof(x.a));
        if (n < 0) error("ERROR writing to socket");
        close(newsockfd);
        return 1;
    }
    error("Client limit exceeded");                             //control shouldn't reach here
    exit(1);
    return 0;
}

int parse(char buffer[])                                  //only /msg supported , WIP
{
    int flag=0;
    if(!strcmp(buffer,"/msg"))
        flag=1;
    return flag;
}
