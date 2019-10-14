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

void search(int client,char s1[])
{
  char s[]="Search\n\0";
 // cout<<"here-func"<<s1;;
  send(client,s,strlen(s),0);

}

void share(int client)
{
  char s[]="share\n";
   send(client,s,strlen(s),0);
}
void invalidinput(int client)
{
  char s[]="Invalid Input\n";
   send(client,s,strlen(s),0);
}

void checkinput(int client)
{
  if (client<0) {
    std::cout<<"Invalid Client"<<std::endl;
  }
}

int main(int argc, char *argv[])
{
   int input_fd,count=0,count1=0;
    ssize_t ret_in;    
    char buff[BUF_SIZE]; 
    char filename[MAXSZ];
     char tosend1[10000];
     char tosend2[10000];
     int center=0;
      ofstream myfile;
      std::time_t result = std::time(NULL);

     struct map hit[100];

 int sockfd;//to create socket
 int newsockfd;//to accept connection

 struct sockaddr_in serverAddress;//server receive on this address
 struct sockaddr_in clientAddress;//server sends to client on this address

 int n, port;
 char msg[MAXSZ];
 socklen_t clientAddressLength;
 int pid;

  port = atoi(argv[1]);

 //create socket
 sockfd=socket(AF_INET,SOCK_STREAM,0);
 //initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
 serverAddress.sin_port=htons(port);

 //bind the socket with the server address and port
 bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));

 //listen for connection from client
 listen(sockfd,5);

 while(1)
 {
  //parent process waiting to accept a new connection
  printf("\n*****server waiting for new client connection:*****\n");
  clientAddressLength=sizeof(clientAddress);
  newsockfd=accept(sockfd,(struct sockaddr*)&clientAddress,&clientAddressLength);
  printf("connected to client: %s\n",inet_ntoa(clientAddress.sin_addr));

  //child process is created for serving each new clients
  pid=fork();
  if(pid==0)//child process rec and send
  {
   //rceive from client
   while(1)
   {
    n=recv(newsockfd,msg,MAXSZ,0);
    if(n==0)
    {
     close(newsockfd);
     break;
    }
    msg[1]='\0';
    //cout<<msg<<endl;
    if (msg[0]=='1')
    {
      // code for search
     char filename[MAXSZ];
     string final="";
     char final2[10000];
     string fname;
     string tos;
     n=recv(newsockfd,filename,MAXSZ,0);
     filename[n]='\0';
     //cout<<filename<<" "<<n<<endl;
     count=0;

    myfile.open("repo.log",std::ios_base::app);
    myfile <<"Search Request from "<<inet_ntoa(clientAddress.sin_addr)<<" at "<<std::asctime(std::localtime(&result))<<endl;
    myfile.close();
   // cout<<"Search Request from"<<inet_ntoa(clientAddress.sin_addr)<<" at "<<std::asctime(std::localtime(&result))<<endl;
    
    input_fd = open ("repo.txt", O_RDONLY);
    if (input_fd == -1) {
            perror ("open");
            exit(0);
    }

    while((ret_in = read (input_fd, &buff, BUF_SIZE)) > 0)
    {
      if(ret_in>0)
      {
        if (buff[0]!='\0')
        {

          tosend1[count1]=buff[0];
          tosend2[count]=buff[0];
          count++;
          count1++;
          if (buff[0]=='\n')
          {
            tosend1[count1-1]='\0';
            tos=tosend1;
            fname=filename;
           // int found = tos.find(fname);
            int found=tos.find(fname);
            //cout<<"("<<found<<")"<<endl;
            count1=0;
          }
         
        }
      }
    }
     tosend2[count]='\0';
     //cout<<tosend2<<endl;
    // search(newsockfd,s1);
    //msg[n]=0;
     //cout<<final;
     //strcpy(final2,final.c_str());
     send(newsockfd,tosend2,strlen(tosend2),0);
    //send(newsockfd,msg,n,0);
    }
    else if(msg[0]=='2')
    {
      char sharefile[MAXSZ];
      string ipv4;
      char ip[MAXSZ]="ipgoeshere\0";
      ipv4=(string)inet_ntoa(clientAddress.sin_addr);
      strcpy(ip,ipv4.c_str());
      char fin_file[MAXSZ]="";
      int k1=0,len=0,i=0,x=0;
      bzero(fin_file,strlen(fin_file));
      int m=recv(newsockfd,sharefile,MAXSZ,0);
      // int len=strlen(filedetails);
       //filedetails[len]='\0';
      len=strlen(sharefile);
      sharefile[m]='\0';
      //cout<<"("<<sharefile<<")";

    myfile.open("repo.log", std::ios_base::app);
    myfile <<"Share Request from "<<inet_ntoa(clientAddress.sin_addr)<<" at "<<std::asctime(std::localtime(&result))<<endl;
    myfile.close();
      while(sharefile[i]!='#')
      {
         fin_file[k1]=sharefile[i];

       k1++;
       i++;
      }
      fin_file[k1]='#';
      k1++;
      // copy ip
      while(x<strlen(ip))
      {
        fin_file[k1]=ip[x];
        k1++;
        x++;
      }
      fin_file[k1]='#';
      k1++;
      i=i+1;
      while(sharefile[i]!='\0')
        {
          fin_file[k1]=sharefile[i];
          k1++;
          i++;
        }
       // k1++;
        fin_file[k1]='#';
        k1++;
        fin_file[k1]='\0';

       
       // write this fin_file to the file itself

        std::ofstream outfile;

       outfile.open("repo.txt", std::ios_base::app);
       outfile << fin_file<<endl; 
       //cout<<fin_file<<endl;

      // code for share
     // cout<<"askdhasldk";
    }
    else if(msg[0]=='3')
    {
      // code for exit
      exit(0);
    }
    else
    {
      // invalid input
      invalidinput(newsockfd);
    }
   
   }//close interior while
  exit(0);
  }
  else
  {
   close(newsockfd);//sock is closed BY PARENT
  }
 }//close exterior while

 return 0;
}
