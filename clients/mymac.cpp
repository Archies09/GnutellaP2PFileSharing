#include "myserver.h"
#include "mycrsclient.h"
#include <thread>
#include <stdio.h>//for fileno
#include <stdlib.h>
#include <unistd.h>//for read
#include <bits/stdc++.h>//for STL
#include <sys/socket.h>//sockets
#include <sys/types.h>//sockets
#include <netinet/in.h>//for socket structure
#include <netinet/tcp.h>//for tcpnodelay
#include <sys/sendfile.h>//for sendfile
#include "myerror.h"

using namespace std;
int main(int argc,char **argv)
{
    if(argc<8)
        catcherror("Invalid arguments");
    char* manc[2];
    manc[0]="YO";
    manc[1]=argv[6];//everyones server port

    thread servt(serv_call,2,manc);
    thread clirt(crscli_call,8,argv);
    servt.join();
    clirt.join();
    //cout<<"THIS IS ALSO THERE"<<endl;
    /*if((pid=fork())<0)
            cerr<<"ERROR IN FORK"<<endl;

    else if(pid==0)//child
    {
        while(1)
        {

        cout<<"Come on Boy.Tell me what u want:"<<endl;
        //char fileloc[BUFSIZ];
        string fileloc;
        scanf("%s",fileloc);
        char *manq[3];
        //manq[0]=fileloc;
        strcpy(manq[0],fileloc.c_str());
        cout<<"IP:";
        string temp;
        cin>>temp;
        strcpy(manq[1],temp.c_str());
        cout<<manq[1]<<endl;
        cout<<"PORT:"<<endl;
        string temprr;
        cin>>temprr;
        strcpy(manq[2],temprr.c_str());
        cout<<manq[2]<<endl;
        cli_call(3,manq);

        }
    }

    else //parent
    {
            cout<<"trolled"<<endl;
            //continue;
            char* manc[2];
            manc[0]="YO";
            manc[1]="5550";
            //serv_call(2,manc);
    }*/

}
