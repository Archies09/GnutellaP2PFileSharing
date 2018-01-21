#include <stdio.h>//for fileno
#include <unistd.h>//for read
#include <bits/stdc++.h>//for STL
#include <sys/socket.h>//sockets
#include <sys/types.h>//sockets
#include <netinet/in.h>//for socket structure
#include <netinet/tcp.h>//for tcpnodelay
#include <sys/sendfile.h>//for sendfile
#include "myerror.h"
//SERVER CODE
using namespace std;


int servy(int argc, char **argv)
{
    struct sockaddr_in serveraddress,clientaddress;/*structure is to store addresses
                                        struct sockaddr_in
                                        { short   sin_family;
                                        u_short sin_port;
                                        struct  in_addr sin_addr;
                                        char    sin_zero[8];   };*/
    int acceptfd,socketfd;


    if(argc<2)
        catcherror("Port Not Provided");

    socketfd=socket(AF_INET,SOCK_STREAM,0);//AF_INET is address domain(for any host on internet) AF_UNIX(for 2 processes sharing common file system) search man for other 2 parameters

    if(socketfd<0)
        catcherror("Socket cannot be created");

    memset(&serveraddress,0,sizeof(serveraddress));
    memset(&clientaddress,0,sizeof(clientaddress));

    int port=atoi(argv[1]);//convert arg[1]to integer;

    //setup address structure
    serveraddress.sin_family=AF_INET;
    serveraddress.sin_port=htons(port);//converts unsigned integer Host byte order TO Network byte order
    serveraddress.sin_addr.s_addr=INADDR_ANY;//sockaddr_contains a structure in_addr containing only one field s_addr. s_addr is address of host which is obtained by INADDR_ANY

    int bindfd=bind(socketfd,(struct sockaddr *)&serveraddress,sizeof(serveraddress));//binds socket to it's address
    if(bindfd<0)
        catcherror("Error in Binding Socket1");//fails if socket is already in use;

    cout<<"I am waiting at"<<port<<endl;
    //connection part is over now listening part
    listen(socketfd,3);//second argument is maximum length of queue of pending connection for socketfd if >3 error connection refused for client

    socklen_t clientlen=(socklen_t)sizeof(clientaddress);
    acceptfd=accept(socketfd,(struct sockaddr *)&clientaddress,&clientlen);//blocks the process until a client connects to server.Wakes up when connection from client is done
    if(acceptfd<0)
        catcherror("Error in Accept");

    int nagle=1;
    setsockopt(acceptfd,IPPROTO_TCP,TCP_NODELAY,(char *) &nagle,sizeof(int));


    char readbuffer[BUFSIZ];//reads characters from connection

    int readbytes=recv(acceptfd,readbuffer,BUFSIZ,0);
    if(readbytes<0)
        catcherror("Error in read at server");
    readbuffer[readbytes]='\0';//to remove end of stream character

    string relpat(readbuffer);

    FILE *sendfileptr;
    string homedir(getenv("HOME"));
    char *mypath=realpath((homedir+relpat).c_str(),NULL);
    cout<<homedir<<"Y"<<endl<<relpat<<"X"<<endl<<mypath<<"C"<<endl;
    sendfileptr=fopen(mypath,"r");
    if(sendfileptr==NULL)
        {printf("File does not exit.Check Path %s\n",readbuffer);exit(1);}
    cout<<"file exists"<<endl;
    cout<<"File Location is";
    for(int i=0;i<readbytes;i++)
        cout<<readbuffer[i];
    cout<<endl;

    fseek(sendfileptr,0,SEEK_END);
    int filesize=ftell(sendfileptr);
    //fseek(sendfileptr,0,SEEK_SET);
    //cout<<"Y"<<ftell(sendfileptr)<<endl;

    char myfilesize[BUFSIZ];
    sprintf(myfilesize,"%d",filesize);

    int tt=send(acceptfd,myfilesize,strlen(myfilesize),0);
    if(tt<0)
            catcherror("Send system call fault1");
    cout<<"Sent "<<tt<<" Bytes to client"<<endl;

    FILE *senfi;
    senfi=fopen(mypath,"r");

    long int senttotal=0;
    int finaltotal=filesize;
    int mylen;
    int servebuffer[BUFSIZ];
    if(finaltotal>=BUFSIZ)
        mylen=BUFSIZ;
    else mylen=finaltotal;
    while(finaltotal>0)
    {
        int rbt=read(fileno(senfi),servebuffer,mylen);
        tt=send(acceptfd,servebuffer,rbt,0);
        finaltotal-=tt;
        cout<<"Sent "<<tt<<" Bytes to client "<<finaltotal<<" Remaining"<<endl;
       	unsigned int timer=1000;
        usleep(timer);
        bzero(servebuffer,BUFSIZ);
    }

    fclose(sendfileptr);
    close(acceptfd);
    return 0;

}


