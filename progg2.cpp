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
 int main()

{
   int fd_fifo; /*дескриптор FIFO*/

  char buffer[]="Текстовая строка для fifo\n";

  char buf[100]; 
if((fd_fifo=open("/tmp/fifo0001.1", O_RDWR)) == - 1)

  {

    fprintf(stderr, "Невозможно открыть fifo\n");

    exit(0);

  } 
while(1)
{

if(read(fd_fifo, &buf, sizeof(buf)) == -1)
  {
  fprintf(stderr, "Невозможно прочесть из FIFO\n");
  }
  else
  {
  printf("Прочитано из FIFO : %d\n",sum);
  }
  
} 
}
