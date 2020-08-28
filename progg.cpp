#include <unistd.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include "sosi.h"
#define ERROR_CREATE_THREAD -11

#define SUCCESS        0

#define our_port   1601
#define our_ip   "127.0.0.1"

using namespace std;


sem_t semaphore;



void* helloWorld(void *args) {
    FILE *fp;
    int sum=0;
    char g;
    string s;
    sosi bibu;
    char name[8]="buf.txt";
    //socket
    int sock;
    struct sockaddr_in server_address;
    sock=socket(AF_INET, SOCK_STREAM,0);
    
    server_address.sin_port=htons(our_port);
    server_address.sin_family=AF_INET;
    inet_pton(AF_INET,our_ip,&server_address.sin_addr);
    
    int ret=connect(sock, reinterpret_cast<struct sockaddr*>(&server_address),sizeof(server_address));
   
    char buffer[1024];
    
    while(1)
    {
    sum=0; 
    sem_wait(&semaphore);
    ifstream in(name);
    in >> s;
   
    in.close();
    bibu.set(s);
    sum=bibu.sum();
    printf ("%d",sum);
     printf("\n");  
    sprintf(buffer,"%d",sum);
    cout << buffer << endl;
    send(sock,buffer,1024,0);
    }    
}
 
int main() {
    FILE *fp;
    pthread_t thread;
    int status;
    int flag;
    int leng;
    char name[8]="buf.txt";
    string s;
    sosi bibu;
    status = pthread_create(&thread, NULL, helloWorld, NULL);
    if (status != 0) {
        printf("main error: can't create thread, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    }      
      while (1)
      {
       printf("write string= \n");
       cin >> s;
       bibu.set(s);
       flag=bibu.check();
       if(flag==1)
       {
        printf("must be only numbers\n");
        continue;
       }
       if(flag==2)
       {
        printf("not more then 64\n");
        continue;
       }
      // sort
      bibu.sort();
      //replace
      bibu.replace(); 
      s=bibu.get();   
      ofstream out(name);
      out << s << endl;      
      out.close();
      sem_post(&semaphore);
      sleep (1);      
      } 
    return 0;
}
