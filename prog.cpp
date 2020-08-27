#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define SHARED_MEMORY_OBJECT_NAME "my_shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE 50
#define SHM_CREATE 1
#define SHM_PRINT  3
#define SHM_CLOSE  4

void func(void* pParams)
{
	int semid;                    /* идентификатор семафора */
	int shmid;                    /* идентификатор разделяемой памяти */
	message_t* msg_p;             /* адрес сообщения в разделяемой
									 памяти */
	char s[SHARED_MEMORY_OBJECT_SIZE];
	if ((semid = semget(SEM_ID, 1, 0)) < 0)
		sys_err("client: can not get semaphore");

	/* получение доступа к сегменту разделяемой памяти */
	if ((shmid = shmget(SHM_ID, sizeof(message_t), 0)) < 0)
		sys_err("client: can not get shared memory segment");

	/* подключение сегмента к адресному пространству процесса */
	if ((msg_p = (message_t*)shmat(shmid, 0, 0)) == NULL)
		sys_err("server: shared memory attach error");

	while (1) {
		if (semctl(semid, 0, GETVAL, 0))     /* блокировка - ждать */
			continue;

		
		if (semctl(semid, 0, GETVAL, 1))     /* блокировка - ждать */
                 {
		semctl(semid, 0, SETVAL, 1); 
	        printf("%s\n", msg_p->string);

			semctl(semid, 0, SETVAL, 1); /* снять блокировку */
		}
	}
}
int main() {


     char s[SHARED_MEMORY_OBJECT_SIZE]
	  while (1)
    {
	
      printf("Введите сообщение: ");
      fgets(s, sizeof(s), stdin);
      s = (s<=SHARED_MEMORY_OBJECT_SIZE)?s:SHARED_MEMORY_OBJECT_SIZE;
        mode = O_CREAT;
        cmd = SHM_CREATE;
      
      semctl (semid, 0, SETVAL, 0);     /* блокировать */
      if (strlen (s) != 1) 
	  {
          strncpy (msg_p->string, s, MAX_STRING);
      }
      semctl (semid, 0, SETVAL, 1);     /* отменить блокировку */
      if (strlen (s) == 1)
        break;
    }
  shmdt (msg_p);                /* отсоединить сегмент разделяемой памяти */
  exit (0);
}
