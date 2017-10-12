#include <stdio.h>
#include <sys/types.h>//socket
#include <sys/socket.h>//socket
#include <string>//memset
#include <string.h>
#include <stdlib.h>//sizeof
#include <netinet/in.h>//INADDR_ANY
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/stat.h> 
#include <fcntl.h>
#include <fstream>
#include <ctime>

#define PORT 8003
#define port2 9050
#define SERVER_IP "127.0.0.1"
#define MAXSZ 10000
#define BUF_SIZE 1

using namespace std;

struct map
  {
    int key;
    char name[300];
    char ip4[300];
    char loc [300];
  };

void printmirrors(char msg[],char filename[])
{
	 int i=0,k=0,j=0,in=0,var=0,n,mirror_num,flag=0;
	 int index=0;
	 string f,s;
	 char newds[1000];
	 char lines[10000];
	 char filesave[500]="./download/\0";
	 char filesave2[500];
	 struct map p[500];
	 ofstream myfile,myfile2;
	 f=filename;
	   std::time_t result = std::time(NULL);
	 while(msg[i]!='\0')
	 {
	 	if(msg[i]!='\n')	
	 	{ 		
	 	lines[k]=msg[i];
	 	k++;
	 }
	 	else
	 	{
	 	lines[k]='\0';	
	 	var=0; 
	 	while(lines[var]!='#')
	 	{
	 		newds[var]=lines[var];
	 		var++;
	 	}	
	 	newds[var]='\0';
	 	s=newds;
	 	int found=s.find(f);
	 	if (found>-1)
	 		// cout<<lines<<found<<endl;
	 		// code for finding and printing mirrors
	 	{
	 		j=0;
	 		in=0;
	 		int len=strlen(lines);
	 		while(lines[j]!='#')
	 		{
	 			p[index].name[in]=lines[j];
	 			j++;
	 			in++;
	 		}
	 		p[index].name[in]='\0';
	 		j++;
	 		in=0;
	 		while(lines[j]!='#')
	 		{
	 			p[index].ip4[in]=lines[j];
	 			j++;
	 			in++;
	 		}
	 		p[index].ip4[in]='\0';
	 		j++;
	 		in=0;
	 		while(lines[j]!='#')
	 		{
	 			p[index].loc[in]=lines[j];
	 			j++;
	 			in++;
	 		}
	 		p[index].loc[in]='\0';
	 		j++;
	 		p[index].key=index;

	 		index++;

	 		//printf("%s ",p[index].loc);
	 	}

	 	k=0;
	 	}
	 	i++;
	 }
	 //clrscr();
	 if (index>0)
	 cout<<"\n::::::Mirrors available::::::\n---------------------------------------------\n";
	else
	{
		cout<<"Mirrors NOT available\n";
		return;
	}
	 for(int h=0;h<index;h++)
	 {
	 	cout<<p[h].key<<"."<<"\t"<<p[h].name<<"\t"<<p[h].ip4<<"\t"<<endl;
	 }
	 cout<<"---------------------------------------------\n\nPlease select a mirror number to download..";
	 cin>>mirror_num;
	 // create another client to connect to the download server
	 int dsock;
	 struct sockaddr_in dserveraddress;
	 dsock=socket(AF_INET,SOCK_STREAM,0);

	 memset(&dserveraddress,0,sizeof(dserveraddress));
 	dserveraddress.sin_family=AF_INET;
 	dserveraddress.sin_addr.s_addr=inet_addr(SERVER_IP);
 	dserveraddress.sin_port=htons(port2);

 	char msg10[10000];
 	char msg20[10000];

 	if(connect(dsock,(struct sockaddr *)&dserveraddress,sizeof(dserveraddress))== -1)
 	{
 	perror("connect");
 	//printf("unable to cbinddd\n" );
	exit(-1);
 	}

 	// while(1)
 	// {
  // printf("\nEnter message to send to server:\n");
  // fgets(msg10,MAXSZ,stdin);
  // if(msg10[0]=='#')
  //  break;

 //cout<<"("<<p[mirror_num].loc<<")"<<endl;
  n=strlen(p[mirror_num].loc)+1;
  send(dsock,p[mirror_num].loc,n,0);

  // update log file
  myfile2.open("client.log",std::ios_base::app);
  myfile2 <<"Download request from "<<inet_ntoa(dserveraddress.sin_addr)<<" at "<<std::asctime(std::localtime(&result))<<endl;
  myfile2.close();
 // n=recv(dsock,msg20,10000,0);
  //printf("\n");
  //if(n>0)
  //{
  	cout<<"\nEnter filename to be saved as:";
  	cin>>filesave2;
 // }
 // printf("[%s]",msg20);
  // now write this msg20 to the file.
  strcat(filesave,filesave2);
 // cout<<filesave;
  flag=0;
  myfile.open(filesave,ios::out|ios::binary);
  while(n=recv(dsock,msg20,sizeof(msg20),0))
  {
  flag=1;
  myfile.write(msg20,n);
  //cout<<msg20;
  }
  myfile.close();
  //wait(0);
  // if(n>0)
  // {
  if(flag==1)
  {
  cout<<"\nFile successfully downloaded at: "<<filesave;
  }
  else
  {
     cout<<"\nError in downloading.";
     remove(filesave);
  }

  // myfile2.open("client.log",std::ios_base::app);
  // myfile2 <<"File is sent to "<<inet_ntoa(dserveraddress.sin_addr)<<" at "<<std::asctime(std::localtime(&result))<<endl;
  // myfile2.close();
// }
 	// }	
}

int main(int argc, char *argv[])
{

	int pid,ppid;

	pid=fork();

	int port1 = atoi(argv[1]);

	if(pid>0 && port1!=port2)
	{
 int sockfd;//to create socket

 struct sockaddr_in serverAddress;//client will connect on this

 int n, port,n1,filel;
 char msg1[MAXSZ];
 char msg2[10000];
 port = atoi(argv[2]);

 //create socket
 sockfd=socket(AF_INET,SOCK_STREAM,0);
 //initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=inet_addr(argv[1]);
 serverAddress.sin_port=htons(port);

 //client  connect to server on port
 if(connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress))== -1)
 {
 	perror("connect");
 	printf("unable to bind\n" );
	exit(-1);
 }
 //send to sever and receive from server
 // /cout<<"Select:\n1. Search\n2. Share\n3. Exit\n";

 while(1)
 {
  cout<<"\nSelect:\n1. Search\n2. Share\n3. Exit\n";
  printf("Enter option:\n");
  cin>>msg1;
  n=strlen(msg1);
  msg1[n]='\0';
  send(sockfd,msg1,n,0);
  char filename[1000];
  char path[1000];
  filel=0;
  switch(msg1[0])
  {
  	case '1': 
  			cout<<"Enter filename to Search:";
  			cin>>filename;  
  			filel=strlen(filename);
  			//cout<<filel;
  			//filename[filel]='\0';			
  			send(sockfd,filename,strlen(filename),0);
  			n1=recv(sockfd,msg2,10000,0);
  			msg2[n1]='\0';
  			//printf("%s\n",msg2);
  			printmirrors(msg2,filename);
  			break;
  	case '2':
  			cout<<"Enter filename to share:";
  			cin>>filename;  
  			cout<<"Enter filepath to share:";
  			cin>>path;
  			strcat(filename,"#");
  			strcat(filename,path);
  			//cout<<filename;
  			send(sockfd,filename,strlen(filename),0);
  			break;
  	case '3':
  			exit(0);
  			break;
  	default:
  			cout<<"Wrong choice";
  }

  
 }
}
else if(pid==0)
{
	//printf("SERVER CODE HERE");
 int input_fd,count=0,count1=0;
    ssize_t ret_in;    
    char buff[BUF_SIZE]; 	

 int sockfd2;//to create socket
 int newsockfd2;//to accept connection
ofstream myfile2;
 std::time_t result = std::time(NULL);

 struct sockaddr_in serverAddress2;//server receive on this address
 struct sockaddr_in clientAddress2;//server sends to client on this address

 int n, port;
 char msg[MAXSZ];
// printf("connected to cclient: %s and port: %d\n",inet_ntoa(clientAddress.sin_addr),port2);
 socklen_t clientAddressLength2;
 // int pid;
 //char msg[MAXSZ];
 //int n;

  port = port2;

 //create socket
 sockfd2=socket(AF_INET,SOCK_STREAM,0);
 //initialize the socket addresses
 memset(&serverAddress2,0,sizeof(serverAddress2));
 serverAddress2.sin_family=AF_INET;
 serverAddress2.sin_addr.s_addr=htonl(INADDR_ANY);
 serverAddress2.sin_port=htons(port2);

 //bind the socket with the server address and port
 bind(sockfd2,(struct sockaddr *)&serverAddress2, sizeof(serverAddress2));

 //listen for connection from client
 listen(sockfd2,5);
 
 while(1)
 {
  //parent process waiting to accept a new connection
  //printf("\n****SServer waiting for new client connection:*****\n");
  //printf("here");
  clientAddressLength2=sizeof(clientAddress2);
  newsockfd2=accept(sockfd2,(struct sockaddr*)&clientAddress2,&clientAddressLength2);
  printf("connected to download server: %s and port: %d\n",inet_ntoa(clientAddress2.sin_addr),port2);
  //printf("here");
  //child process is created for serving each new clients
  ppid=fork();
  if(ppid==0)//child process rec and send
  {

   //rceive from client
   // while(1)
   // {
    n=recv(newsockfd2,msg,MAXSZ,0);
    if(n==0)
    {
     close(newsockfd2);
     break;
    }    
    msg[n]=0;
    //filename+loc is stored in msg
    // //open msg and send its buffer together
     input_fd = open (msg, O_RDONLY);
     char tosend[10000];
     int cntr=0;
     while((ret_in = read (input_fd, &buff, BUF_SIZE)) > 0)
    {
      if(ret_in>0)
      {
    	tosend[0]=buff[0];
    	tosend[1]='\0';    	
    	 send(newsockfd2,buff,BUF_SIZE,0);
    }
    }
    //tosend[cntr]='\0';	
    //tosend[0]='X';
    //printf("Receive and set:%s\n",tosend);
    //msg[0]='H';
    
   // send(newsockfd2,tosend,cntr,0);
  myfile2.open("client.log",std::ios_base::app);
  myfile2 <<"File Sent to "<<inet_ntoa(clientAddress2.sin_addr)<<" at "<<std::asctime(std::localtime(&result))<<endl;
  myfile2.close();

   // }//close interior while
  exit(0);
  // close(newsockfd2);
  }
  else
  {
   close(newsockfd2);//sock is closed BY PARENT
  }
  }//close exterior while

 

}


 return 0;
}