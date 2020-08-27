#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>


#define SEM_ID	2001      /* ключ массива семафоров */
#define SHM_ID	2002      /* ключ разделяемой памяти */
#define PERMS	0666      /* права доступа */

/* коды сообщений */

#define MSG_TYPE_EMPTY  0 /* пустое сообщение */
#define MSG_TYPE_STRING 1 /* тип сообщения о том, что
							 передана непустая строка */
#define MSG_TYPE_FINISH 2 /* тип сообщения о том, что
							 пора завершать обмен */
                             
#define MAX_STRING	64

/* структура сообщения, помещаемого в разделяемую память */
typedef struct
{
  char string [MAX_STRING];
} message_t; 

void sys_err (char *msg)
{
  puts (msg);
  exit (1);
}

void func(void* pParams)
{
	int semid;                    /* идентификатор семафора */
	int shmid;                    /* идентификатор разделяемой памяти */
	message_t* msg_p;             /* адрес сообщения в разделяемой
									 памяти */
	char s[MAX_STRING];
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

		if (msg_p->type != MSG_TYPE_EMPTY)
		{
			if (semctl(semid, 0, GETVAL, 0))     /* блокировка - ждать */
				continue;

			semctl(semid, 0, SETVAL, 1); /* установить блокировку */

			/* обработка сообщения */
			
				printf("%s\n", msg_p->string);

			semctl(semid, 0, SETVAL, 0); /* снять блокировку */
		}
	}
}
int main() {

	int semid, shmid;

	  /* создание массива семафоров из одного элемента */
	  if ((semid = semget (SEM_ID, 1, PERMS | IPC_CREAT)) < 0)
	    sys_err ("server: can not create semaphore");

	  /* создание сегмента разделяемой памяти */
	  if ((shmid = shmget (SHM_ID, sizeof (message_t), PERMS | IPC_CREAT)) < 0)
	    sys_err ("server: can not create shared memory segment");
	  if ((msg_p = (message_t*)shmat(shmid, 0, 0)) == NULL)
		  sys_err("client: shared memory attach error");
	  semctl(semid, 0, SETVAL, 0);     /* блокировать */
          _beginthread ( func,0,NULL);
	  while (1)
    {
	
      printf("Введите сообщение: ");
      fgets(s, sizeof(s), stdin);


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
