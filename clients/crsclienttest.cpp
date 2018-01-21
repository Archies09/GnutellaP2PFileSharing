#include <unistd.h>//for read
#include <bits/stdc++.h>//for STL
#include <sys/socket.h>//sockets
#include <sys/types.h>//sockets
#include <netinet/in.h>//for socket structure
#include <netdb.h>//for hostnet
#include <fstream>
#include <iostream>
#include "myerror.h"
#include "myclient.h"
//CLIENT CODE
using namespace std;

/*void catcherror(char *message)
{
    cerr<<message<<endl;
    exit(1);
}*/

int crscliy(int argc, char **argv)
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

    //if(argc<3)
      //  catcherror("Wrong Usage");

    socketfd=socket(AF_INET,SOCK_STREAM,0);//AF_INET is address domain(for any host on internet) AF_UNIX(for 2 processes sharing common file system) search man for other 2 parameters
    if(socketfd<0)
        catcherror("Socket cannot be created");

    memset(&serveraddress,0,sizeof(serveraddress));

    serverhost=gethostbyname(argv[4]);//name of host is converted to structure hostent containing information about host. Performs lookup in the configuration files /etc/host.conf and /etc/nsswitch.conf.
    if(serverhost==NULL)
        catcherror("No Such Host");

    int port=atoi(argv[5]);//convert arg[1]to integer;

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
   while(1)
    {

    cout<<"Enter the command:";
    //string fileloc;
    scanf("\n");
    fgets(argv[0],100,stdin);
    cout<<argv[0];

    char inpu[BUFSIZ];
    //cin>>fileloc;
    strcpy(inpu,argv[0]);
    int writebytes=send(socketfd,inpu,BUFSIZ,0);
    if(writebytes<0)
        catcherror("No msg sent");


    int readbytes;
    char readbb[BUFSIZ];
    memset(&readbb,0,BUFSIZ);
    //string outpu="";
    readbytes=recv(socketfd,readbb,BUFSIZ,0);
        //for(int i=0;i<readbytes;i++)
            //outpu.append(readbb);
 

    if(argv[0][0]=='g')
    {
	cout<<readbb<<endl;
	string fname,ipd;
	char **pq;
            pq = (char **)malloc(sizeof(char *) * 100);
            for (int i=0;i<100;i++)
            {

                pq[i] = (char *)malloc(sizeof(char) * 100);
            }
	int mp=0;
	while(readbb[mp]!=':')
	{
		fname+=readbb[mp];
		mp++;
	}
	mp++;
	while(mp<readbytes)
	{
		ipd+=readbb[mp];
		mp++;
	}
	strcpy(pq[0],fname.c_str());
        strcpy(pq[1],ipd.c_str());
	cout<<pq[0]<<endl<<pq[1]<<endl;
	cli_cll(2,pq);
    }	

    cout<<readbb<<endl;

    }
    close(socketfd);

    return 0;

}
