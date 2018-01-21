#include <unistd.h>//for read
#include <bits/stdc++.h>//for STL
#include <sys/socket.h>//sockets
#include <sys/types.h>//sockets
#include <netinet/in.h>//for socket structure
#include <netdb.h>//for hostnet
#include <fstream>
#include <iostream>
#include "clienttest.h"
#include "myerror.h"
//CLIENT CODE
using namespace std;


int cli_cll(int argc,char *argv[100])
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

    char *manq[100];
    //cout<<"Come on Boy.Tell me what u want:"<<endl;
    //string fileloc;
    //scanf("%s",manq[0]);
	manq[0]=argv[0];
    //cin>>fileloc;
    //strcpy(manq[0],fileloc.c_str());//strcpy(manq[0],fileloc.c_str());
    //cout<<"IP:"<<endl;
    //scanf("%s",manq[1]);
	manq[1]=argv[1];    
	cout<<"Port:"<<endl;
    scanf("%s",manq[2]);
    //printf("%s",*manq[2]);
  //  if(argc<3)
//        catcherror("Wrong Usage1");

    socketfd=socket(AF_INET,SOCK_STREAM,0);//AF_INET is address domain(for any host on internet) AF_UNIX(for 2 processes sharing common file system) search man for other 2 parameters
    if(socketfd<0)
        catcherror("Socket cannot be created");

    memset(&serveraddress,0,sizeof(serveraddress));

    serverhost=gethostbyname(manq[1]);//name of host is converted to structure hostent containing information about host. Performs lookup in the configuration files /etc/host.conf and /etc/nsswitch.conf.
    if(serverhost==NULL)
        catcherror("No Such Host");

    int port=atoi(manq[2]);//convert arg[1]to integer;

    //setup address structure
    serveraddress.sin_family=AF_INET;
    serveraddress.sin_port=htons(port);//converts unsigned integer Host byte order TO Network byte order
    bcopy((char *)serverhost->h_addr,(char *)&serveraddress.sin_addr.s_addr,serverhost->h_length);//copies serverhost address to s_addr

    //connect
    socklen_t serverlen=(socklen_t)sizeof(serveraddress);
    int connectifd=connect(socketfd,(struct sockaddr *)&serveraddress,serverlen);//connects to server
    if(connectifd<0)
        catcherror("Error in Connecting");
    recv(socketfd,manq[2],4,0);
    cliy(3,manq);
    close(socketfd);

}
