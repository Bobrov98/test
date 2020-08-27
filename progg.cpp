#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 




#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0
#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE 96
#define SHM_CREATE 1
#define SHM_PRINT  3
#define SHM_CLOSE  4 
using namespace std;
sem_t semaphore;
static int counter = 0;


void* helloWorld(void *args) {
    FILE *fp;
    int sum=0;
    char g;
    char s[SHARED_MEMORY_OBJECT_SIZE];
    int fd_fifo; /*дескриптор FIFO*/

    char buffer[]="Текстовая строка для fifo\n";

    char buf[100];

   

  /*Если файл с таким именем существует, удалим его*/

     unlink("/tmp/fifo0001.1");

  /*Создаем FIFO*/

    if((mknod("/tmp/fifo0001.1",S_IFIFO|0x1b6,0)) == -1)

    {

    fprintf(stderr, "Невозможно создать fifo\n");

    exit(0);

    }

  /*Открываем fifo для чтения и записи*/

    if((fd_fifo=open("/tmp/fifo0001.1",S_IFIFO|0x1b6,0)) == - 1)

    {

    fprintf(stderr, "Невозможно открыть fifo\n");

    exit(0);

    } 

    while(1)
    {
    sum=0;
    printf ("%d",sum);
    sem_wait(&semaphore);
    printf("\n");
    fp=fopen("buf.txt","r+");
    fscanf (fp,"%s",s);
    printf ("%s",s);
    fclose (fp);
    fp=fopen("buf.txt","w"); 
    fclose (fp);
    printf("\n");
    for (int i=0; i<=strlen(s);i++)
    {
     g=s[i];
     if (g>='0' && g<='9')
        {
         sum+=g-48;
        }
    }  
    printf ("%d",sum);
     printf("\n");
     write(fd_fifo,buffer,strlen(buffer)) ;
    }    
}
 
int main() {
    FILE *fp;
    pthread_t thread;
    int status;
    int flag;
    int leng;
    char g;
    int n1, n2;
    int status_addr;
    int shm, len, cmd, mode = 0;
    char name[8]="buf.txt";
    char s[SHARED_MEMORY_OBJECT_SIZE];
    char s2[SHARED_MEMORY_OBJECT_SIZE];
    status = pthread_create(&thread, NULL, helloWorld, NULL);
    if (status != 0) {
        printf("main error: can't create thread, status = %d\n", status);
        exit(ERROR_CREATE_THREAD);
    } 
      
   
     
      while (1)
      {
       printf("write string= \n");
       fgets (s, sizeof(s),stdin);
       flag=0;
       leng=strlen(s);
       fp=fopen("buf.txt","w"); 
       for (int i=0; i<=64; i++)
       {
        g=s[i];
        // printf ("%c",g);
        if (g<'0'||g>'9')
        {
         flag=1;
         break;
        }
        if (strlen(s)>64)
        {
         flag=2;
         break;
        }
        if (g='\n')
        {
         break;
        }
       }
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
      //replace
      n1=0;
      n2=0;
      while (s[n1]!='\n')
      {
      if (n1%2==0)
         {
          s2[n2]='K';
          n2++;
          s2[n2]='B';
          n2++;
          n1++;
         }
      else
         {
          s2[n2]=s[n1];
          n2++;
          n1++;
         }
      }
      s2[n2]='\n';
      
      
      fprintf (fp,"%s",s2);
      fclose (fp);
      sem_post(&semaphore);
      sleep (1);
      
      }
  
    return 0;
}
