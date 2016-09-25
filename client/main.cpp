//
//  main.cpp
//  client
//
//  Created by Anant Prakash on 21/09/16.
//  Copyright © 2016 Anant Prakash. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

char comds[][10]={"/quit","/msg","/receive","/join","/nick"};   //commands supported , WIP
struct data
{
    char a[256],name[100];
    
};
int parse(char buffer[]);

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno;
    //int n;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    if (argc < 3) {
        cerr<<"usage "<<argv[0]<<" hostname port\n";
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        cerr<<"ERROR, no such host\n";
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    int flag=1;                                                 //signals termination if 0
    data x;
    cout<<"enter your nickname:";
    cin>>x.name;
    while(flag)
    {   int n;
        cout<<"Connected with server\n";
        bzero(buffer,256);
        cout<<"enter command:";
        fgets(buffer,255,stdin);                                 //fgets prevents overflow
        flag=parse(buffer);                                     //parses the msg and decides what to do
        switch(flag)
        {
            case 1:
                cout<<"enter your message :";
                cin>>*(x.a);
                n = write(sockfd,x.name,sizeof(x.name));
                if (n < 0)
                    error("ERROR writing to socket");
                n = write(sockfd,x.a,sizeof(x.a));
                if (n < 0)
                    error("ERROR writing to socket");
                break;
            case 2:
                bzero(x.name,sizeof(x.name));
                n = read(sockfd,x.name,sizeof(x.name));
                if (n < 0)
                    error("ERROR reading from socket");
                bzero(x.a,sizeof(x.a));
                n = read(sockfd,x.a,sizeof(x.a));
                if (n < 0)
                    error("ERROR reading from socket");
                cout<<"messege from "<<x.name<<": "<<(x.a)<<"\n";
                break;
        }
        flag=0;
        /*
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
            error("ERROR writing to socket");
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("%s\n",buffer);
         */
    }
    
    close(sockfd);
    return 0;
}

int parse(char buffer[])
{
    int flag=0;
    for(int i=0;i<3 ;i++)
    {
        if (strcmp(buffer,comds[i])) flag=i;
        break;
    }
    return flag;
}
