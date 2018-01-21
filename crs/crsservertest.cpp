#include <stdio.h>//for fileno
#include <unistd.h>//for read
#include <bits/stdc++.h>//for STL
#include <sys/socket.h>//sockets
#include <sys/types.h>//sockets
#include <netinet/in.h>//for socket structure
#include <netinet/tcp.h>//for tcpnodelay
#include <sys/sendfile.h>//for sendfile
#include "myerror.h"
#include <string>
#include <libgen.h>
#include <arpa/inet.h>

//SERVER CODE
using namespace std;


int servy(int argc, char **argv)
{

    FILE *repofile;
    FILE *editrepo;
    repofile=fopen(argv[3],"r");
    vector <string> vv;
    if(repofile>0)
    {
        char rrfile[BUFSIZ];
        while(fgets(rrfile,BUFSIZ,repofile)!=NULL)
        {
            vv.push_back(rrfile);
            memset(&rrfile,0,strlen(rrfile));
        }
        editrepo=fopen(argv[3],"a");

    }
    else
    {
	cout<<"FILE WAS EMPTY"<<endl;
        editrepo=fopen(argv[3],"w");
    }



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

    int port=atoi(argv[2]);//convert arg[1]to integer;

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
    string ops;

    while(1)
    {
    cout<<"ENTERED"<<endl;

    char cliip[100];
    getpeername(socketfd,(struct sockaddr*)&clientaddress,&clientlen);
    struct sockaddr_in *cliptr=(struct sockaddr_in *)&clientaddress;
    inet_ntop(AF_INET,&cliptr->sin_addr,cliip,sizeof(cliip));




    int nagle=1;
    setsockopt(acceptfd,IPPROTO_TCP,TCP_NODELAY,(char *) &nagle,sizeof(int));


    char readbuffer[BUFSIZ];//reads characters from connection

    int readbytes=recv(acceptfd,readbuffer,BUFSIZ,0);
    if(readbytes<0)
        catcherror("Error in read at server");
    readbuffer[strlen(readbuffer)-1]='\0';//to remove end of stream character
    string checker(readbuffer);
    //checker.copy(readbuffer,BUFSIZ,0);
    regex searchtemp ("(search)(.*)");
    regex deltemp ("(del)(.*)");
    regex sharetemp ("(share)(.*)");
    regex gettemp ("(get)(.*)");
    cout<<checker<<endl;

    if(regex_match(checker,searchtemp))
    {
        string subss=checker.substr(8,checker.length()-9);
        cout<<subss<<endl;
        string subsslow;
        for(int i=0;i<subss.length();i++)
                subss[i]=tolower(subss[i]);
        subsslow.append(subss);
        cout<<"X"<<subsslow<<endl;
        //subsslow.append("\n");

        for(int it=0;it!=vv.size();it++)
        {
            string svs(vv[it]);
            string mytry;
            cout<<svs<<endl;
            for(int i=0;i<vv[it].length();i++)
                    svs[i]=tolower(svs[i]);
            cout<<"Y"<<svs;
            mytry.append(svs);
            //svs.append("\n");
            cout<<mytry<<endl;
            mytry.erase(mytry.length()-1);

            if(mytry.find(subsslow)!=string::npos)
            {

                //cout<<"HELL"<<endl;
                string svsold(vv[it]);
                //cout<<svsold<<endl;
                ops.append(svsold);
                //zops.append("\n");

            }

        }
        send(acceptfd,ops.c_str(),ops.length(),0);

    }
    else if(regex_match(checker,deltemp))
    {
       FILE *tr;
       tr=fopen(argv[3],"r");
       cout<<"INSIDE"<<endl;
       string tempoold=checker.substr(5,checker.length()-6);
       char *myfilename=new char[100];
       myfilename=basename((char *)tempoold.c_str());
       string finalipt(myfilename);
       finalipt.append(":");
       finalipt.append(tempoold);
       string tempo(finalipt);
       cout<<tempo<<endl<<finalipt<<endl;
       char mycheck[BUFSIZ];
       FILE *tempfile;
       tempfile=fopen("tmpfile","w");
       int kk=0;
       tempo.append("\n");
       finalipt.append("\n");
       finalipt.append("\n");
       while((read(fileno(tr),mycheck,vv[kk].length()))>0)
       {
           cout<<mycheck<<endl;
           if(strcmp(tempo.c_str(),mycheck)==0)
            vv.erase(vv.begin()+kk);
           else if(strcmp(finalipt.c_str(),mycheck)==0)
            vv.erase(vv.begin()+kk);
           else{
            write(fileno(tempfile),mycheck,vv[kk].length());
            //write(fileno(tempfile),"\n",1);
            }
            memset(mycheck,0,vv[kk].length());
           kk++;
       }
       remove(argv[3]);
       rename("tmpfile",argv[3]);
       send(acceptfd,"DELETED SUCCESSFULLY",20,0);
       fclose(tempfile);
       fclose(tr);
    }

    else if(regex_match(checker,sharetemp) )
    {
	//cout<<"YOYO"<<endl;
	FILE *rtt;
	rtt=fopen(argv[3],"a");
	string tempchecker;
	string tempo;
	tempo=checker.substr(7,checker.length()-8);
	char *myfilename=new char[100];
    myfilename=basename((char *)tempo.c_str());
    string finalipt(myfilename);
    finalipt.append(":");
    finalipt.append(tempo);
    finalipt.append(":");
    string ipp(cliip);
    finalipt.append(ipp);
    vv.push_back(finalipt);
	write(fileno(rtt),finalipt.c_str(),finalipt.length());
	write(fileno(rtt),"\n",1);
     //   cout<<tempo<<endl;

    send(acceptfd,"SHARED SUCCESSFULLY",19,0);
    cout<<"DONE"<<endl;
    fclose(rtt);
    }
    else if(regex_match(checker,gettemp))
    {
        cout<<strlen(checker.c_str())<<endl;

        cout<<"T"<<ops<<"T"<<endl;
        string numb=checker.substr(4,checker.length()-4);
        cout<<"X"<<numb<<endl;
        int num = atoi(numb.c_str());
        cout<<num<<endl;
        int b=0;
        for(int p=1;p<num;p++)
        {
            while(ops[b]!='\n')
                b++;
            while(ops[b]=='\n')
                b++;
        }
        cout<<"T"<<ops<<"T"<<endl;
        cout<<ops[b]<<endl;
        while(ops[b]!='\n')
        {
            while (ops[b]!=':')
                b++;
            b++;
            string fname;
            while (ops[b]!=':')
            {
                fname+=ops[b];
                b++;
            }
            b++;
            string ipd;
            while (ops[b]!='\n')
            {
                ipd+=ops[b];
                b++;
            }
            cout<<"Y"<<fname<<endl<<"Z"<<ipd<<endl;
            char **pq;
            pq = (char **)malloc(sizeof(char *) * 100);
            for (int i=0;i<100;i++)
            {

                pq[i] = (char *)malloc(sizeof(char) * 100);
            }
            strcpy(pq[0],fname.c_str());
            strcpy(pq[1],ipd.c_str());
            string res;
	    res+=pq[0];
	    res+=":";
	    res+=pq[1];
	    cout<<res<<endl;
	    send(acceptfd,res.c_str(),res.length(),0);
	    
        }

    }
    else
    {

    }

    }

    shutdown(acceptfd,SHUT_RDWR);
    close(acceptfd);


    return 0;

}


