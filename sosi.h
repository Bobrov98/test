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



using namespace std;
class sosi 
{
private:
string s;
int leng;
public:
 void set(string str)
 {
  s=str;
  leng=str.size();
 
 }
 void sort ()
  {
    char g;
    for (int i=1; i<leng; i++)
       {
        for (int j=0; j<leng-i; j++)
        {
         if (s[j]<s[j+1])
         {
          g=s[j];
          s[j]=s[j+1];
          s[j+1]=g;
         }  
        }
       }     
  }

 void replace ()
 {
  string s2="B";
  char g;
  for (int i=1; i<leng; i++)
       {
          s[i]='K';
          s.insert(i+1,s2);
          i=i+2;  
          leng=s.size();  
       }
  
 }

int check ()
{
 int status=0;
 char g;
 for (int i=0; i<leng; i++)
       {
        g=s[i];
        if (g<'0'||g>'9')
        {
         status=1;
         break;
        }
        if (leng>64)
        {
         status=2;
         break;
        }
        if (g='\n')
        {
         break;
        }
       }
  return status;
}
string get()
{
 return s;
}
int sum ()
{
char g;
int sum=0;
for (int i=0; i<=s.size();i++)
    {
     g=s[i];
     if (g>='0' && g<='9')
        {
         sum+=g-48;
        }
    }  
return sum;
}
};
