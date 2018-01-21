#include <unistd.h>//for read
#include <bits/stdc++.h>//for STL
#include <sys/socket.h>//sockets
#include <sys/types.h>//sockets
#include <netinet/in.h>//for socket structure
#include <netdb.h>//for hostnet
#include <fstream>
#include <iostream>
#include "myerror.h"
//CLIENT CODE
using namespace std;

/*void catcherror(char *message)
{
    cerr<<message<<endl;
    exit(1);
}*/

int cliy(int argc, char argv[][100])
{
    struct sockaddr_in serveraddress;/*structure is to store addresses
                                        struct sockaddr_in
                                        { short   sin_family;
                                        u_short sin_port;
                                        struct  in_addr sin_addr;
                                        char    sin_zero[8];   };*/
    int socketfd;

    struct hostent *serverhost;/*
                                struct  hostent
                                char *h_name;                    official name of host
                                char **h_aliases;                alias list
                                int h_addrtype;                  host address type
                                int h_length;                    length of address
                                char **h_addr_list;              list of addresses from name server in c99 all pointers were char * hence char **
                                #define h_addr  h_addr_list[0]   address, for backward compatiblity
                                */

    if(argc<3)
        catcherror("Wrong Usage");

    socketfd=socket(AF_INET,SOCK_STREAM,0);//AF_INET is address domain(for any host on internet) AF_UNIX(for 2 processes sharing common file system) search man for other 2 parameters
    if(socketfd<0)
        catcherror("Socket cannot be created");

    memset(&serveraddress,0,sizeof(serveraddress));

    serverhost=gethostbyname(argv[1]);//name of host is converted to structure hostent containing information about host. Performs lookup in the configuration files /etc/host.conf and /etc/nsswitch.conf.
    if(serverhost==NULL)
        catcherror("No Such Host");

    int port=atoi(argv[2]);//convert arg[1]to integer;

    //setup address structure
    serveraddress.sin_family=AF_INET;
    serveraddress.sin_port=htons(port);//converts unsigned integer Host byte order TO Network byte order
    bcopy((char *)serverhost->h_addr,(char *)&serveraddress.sin_addr.s_addr,serverhost->h_length);//copies serverhost address to s_addr

	sleep(2);
    cout<<"Sending at "<<port<<endl;
    //connect
    socklen_t serverlen=(socklen_t)sizeof(serveraddress);
    int connectifd=connect(socketfd,(struct sockaddr *)&serveraddress,serverlen);//connects to server
    if(connectifd<0)
        catcherror("Error in Connecting");

    //connection part is over now writing part
    //cout<<"File Location:";
    char fileloc[BUFSIZ];
    //cin>>fileloc;
    strcpy(fileloc,argv[0]);
    int writebytes=send(socketfd,fileloc,BUFSIZ,0);

    char myfilename[BUFSIZ];
    int j=0;

    int i=0;
    for(i=0;fileloc[i]!='\0';i++);
    i--;
    for(;fileloc[i]!='/';i--)
            myfilename[j++]=fileloc[i];
    if(writebytes<0)
        catcherror("Error in write from client");
    myfilename[j]='\0';

    char filecounterstr[BUFSIZ];
    recv(socketfd,filecounterstr,BUFSIZ,0);
    int filecounter=atoi(filecounterstr);
    char dirr[1000];
    getcwd(dirr,sizeof(dirr));
    for(i=0;dirr[i]!='\0';i++);
    dirr[i++]='/';
    for(int k=0;myfilename[k]!='\0';k++)
        dirr[i++]=myfilename[k];
    dirr[i]='\0';
    cout<<dirr<<endl;
    //ofstream writetofi;
    //writetofi=fopen(myfilename,"w");
    //writetofi.open(dirr);
    cout<<filecounter<<endl;
    //memset(fileloc,0,1024);

    FILE *writetofi;
    writetofi=fopen(dirr,"w+");
    int total=filecounter;
    int signalsize=0;
    char recvbuffer[BUFSIZ];
    int tmem;
    if(total<BUFSIZ)
        tmem=total;
    else tmem=BUFSIZ;
    while(((signalsize=recv(socketfd,recvbuffer,tmem,0))>0) && total>0)//
    {
        //fwrite(recvbuffer,sizeof(char),signalsize,writetofi);
        //writetofi<<recvbuffer;
        for(int i=0;i<signalsize;i++)
            fprintf(writetofi,"%c",recvbuffer[i]);

        total-=signalsize;
        cout<<"Received "<<signalsize<<" Bytes from server "<<total<<" Remaining"<<endl;
    }
    close(socketfd);
    fclose(writetofi);

    return 0;

}
