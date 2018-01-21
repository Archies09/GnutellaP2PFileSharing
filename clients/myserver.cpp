#include <stdio.h>//for fileno
#include <unistd.h>//for read
#include <bits/stdc++.h>//for STL
#include <sys/socket.h>//sockets
#include <sys/types.h>//sockets
#include <netinet/in.h>//for socket structure
#include <netinet/tcp.h>//for tcpnodelay
#include <sys/sendfile.h>//for sendfile
#include "servertest.h"
#include "myerror.h"
#include <thread>
//SERVER CODE
using namespace std;

int serv_call(int argc, char **argv)
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
        catcherror("Error in Binding Socket");//fails if socket is already in use;

    //cout<<"SERVER IS LISTENING at "<<port<<endl;
    //connection part is over now listening part
    listen(socketfd,3);//second argument is maximum length of queue of pending connection for socketfd if >3 error connection refused for client

    int counting=6668;
    socklen_t clientlen=(socklen_t)sizeof(clientaddress);

    while((acceptfd=accept(socketfd,(struct sockaddr *)&clientaddress,&clientlen))>0)//blocks the process until a client connects to server.Wakes up when connection from client is done)
    {


        if(acceptfd<0)
            catcherror("Error in Accept2");

        int nag=1;
        setsockopt(acceptfd,IPPROTO_TCP,TCP_NODELAY,(char *) &nag,sizeof(int));
        //cout<<"BROKEN HERE"<<endl;


            char* manu[2];
            char trtr[4];
            counting++;
            sprintf(trtr,"%d",counting);
            cout<<trtr<<endl;
            send(acceptfd,trtr,4,0);
            manu[0]="YO";

            manu[1]=trtr;
            //manu[1]="6669";
            //servy(2,manu);




        thread servt(servy,2,manu);
        //thread clirt(cli_call);
        servt.detach();
        //clirt.join();
        close(acceptfd);



       /* int pid;
        if((pid=fork())<0)
        {
            close(acceptfd);
            cerr<<"ERROR IN FORK"<<endl;
            continue;
        }


        else if(pid==0)//child
        {
            char* manu[2];
            char trtr[4];
            counting++;
            sprintf(trtr,"%d",counting);
            cout<<trtr<<endl;
            send(acceptfd,trtr,4,0);
            manu[0]="YO";

            manu[1]=trtr;
            //manu[1]="6669";
            servy(2,manu);
            close(acceptfd);
            break;
        }

        else //parent
        {
            close(acceptfd);
            cout<<"PARENTTT"<<endl;
            continue;
        }
        */

    }


    return 0;

}


