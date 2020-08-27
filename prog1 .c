#include <linux/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>

int start = 0;

#define LEN 20

pid_t chpid [2];

int main (int argc, char *argv [])
{
struct timeval c_time;
void c_action (int);
static struct sigaction pact, cact;
char str [LEN];
id_t pid;


shm_unlink(«myshered.shm»);

int fd_shm;

if((fd_shm = shm_open(«myshered.shm», O_RDWR|O_CREAT, 0777)) < 0)
{perror («error create shm»); return(1);}


// создаем семафор
int j,n,i=0;


int semid = semget(IPC_PRIVATE, 4, 0666 | IPC_CREAT );
if (semid < 0){
perror(«semget»);
return 1;}

struct sembuf s_red[2] = {0, -1, IPC_NOWAIT, 1, 1, IPC_NOWAIT};
struct sembuf s_green[2] = {0, 1, IPC_NOWAIT, 1, -1, IPC_NOWAIT};
struct sembuf s_wait[2] = {0, 0, 0, 1, 0, 0};
struct sembuf s_readed = {2, 0, 0};
struct sembuf s_lock = {2, 1, IPC_NOWAIT};
struct sembuf s_unlock = {2, -1, IPC_NOWAIT};

union semun arg;

arg.val = 0;
semctl(semid, 0, SETVAL, arg);
arg.val = 1;
semctl(semid, 1, SETVAL, arg);
arg.val = 0;
semctl(semid, 2, SETVAL, arg);


// создаем два процесса
for (j=0; j<2; j++) {
switch (chpid [j] = fork ()) {
case -1:
printf («Fork call error\n»);
return 1;
case 0:
//child
cact.sa_flags = SA_SIGINFO;
cact.sa_sigaction = (void *) c_action;
sigaction (SIGUSR1, &cact, NULL);
printf(«child #%d\n»,getpid());
while (i <= 1000) {
// ожидаем разрешения

if ((start)&&
(semop( semid, &s_readed, 1)==0)&&
(semop( semid, &s_wait[j], 1)==0))
{
lseek (fd_shm, 0, SEEK_END);
i = semctl(semid, 3, GETVAL);
if (i==1000)break;
for (n=0;n<100;n++){
gettimeofday(&c_time,NULL);
sprintf (str,«%4d #%5d :%6d\n», i+n+1,
getpid (), c_time.tv_usec);
write(fd_shm,str,LEN);
}
i+=n;
arg.val = i;
semctl(semid, 3, SETVAL, arg);
// переключаем поток
if (j==0) semop( semid, &s_green, 2);
else semop( semid, &s_red, 2);

}
}
return 0;
}
}
printf(«father #%d\n»,getpid());
//father
kill (chpid [0], SIGUSR1);
kill (chpid [1], SIGUSR1);
i=0;
while (i < 1000) {
sleep(1);
// ожидаем разрешения
j=semctl(semid, 3, GETVAL);
if( (j>i+75)||(j==1000))
{
// блокируем
semop( semid, &s_lock, 1);
// читаем
lseek (fd_shm, i*LEN, SEEK_SET);

for (n=0;n<75;n++){
if (i+n ==1000) break;
read(fd_shm,str,LEN);
printf («%s», str);
}
i+=n;
// разблокируем
semop( semid, &s_unlock, 1);
}
}
/*close(fd);*/

shm_unlink(«myshered.shm»);
semctl(semid, 0, IPC_RMID);
return 0;
}

void c_action (int sig)
{
start = 1;
printf(«action #%d\n»,getpid());
}
