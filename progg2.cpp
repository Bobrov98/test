#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>






#define our_port   1601
using namespace std;
 int main()

{
   int ser;
   int sock;
   int number;
    struct sockaddr_in server_address;
    sock=socket(AF_INET, SOCK_STREAM,0);
    if(sock<0)
    {
      cout<<"socket error";
      exit(0);
    }
    cout<<"socket created \n";
    server_address.sin_port=htons(our_port);
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=htons(INADDR_ANY);
    
    int ret=bind(sock, reinterpret_cast<struct sockaddr*>(&server_address),sizeof(server_address));
    if(ret<0)
    {
      cout<<"biding coonection. Socket has alredy been establishing\n";
      return -1;
    }
    socklen_t size=sizeof(server_address);
    cout<<"listening clients... \n";
    listen(sock, 1);
    ser=accept(sock,reinterpret_cast<struct sockaddr*>(&server_address),&size);
     if(sock<0)
    {
      cout<<"cant accept";
     
    }
    char buffer[1024];
    while(ser>0)
    {
     recv(ser,buffer,1024,0);
     cout << buffer<< endl;
     number=atoi(buffer);
     if (number>=10 && number%32==0)
        {
          cout << buffer<< endl;
        }
     else
     {
        cout <<"error:not right sum"<< endl;
     }    
    }


}
