#include "lib.h"
#include "types.h"

void produce_and_consume(int num) {
	int flag = 0;
	sem_t empty, full, mutex;
	sem_init(&empty, 5);
	sem_init(&full, 0);
	sem_init(&mutex, 1);
	for (int i = 0; i < num; ++i) {
		if (flag == 0)
			flag = fork();
		else if (flag > 0)
			break;
	}
	int id = get_pid();
	if (id == 1) {
		while (1) {
			sem_wait(&full);
			sleep(128);
			sem_wait(&mutex);
			sleep(128);
			printf("Consumer : consume\n");
			sem_post(&mutex);
			sleep(128);
			sem_post(&empty);
			sleep(128);
		}
	}
	else {
		while (1) {
			sem_wait(&empty);
			sleep(128);
			sem_wait(&mutex);
			sleep(128);
			printf("Producer %d: produce\n", id - 1);
			sem_post(&mutex);
			sleep(128);
			sem_post(&full);
			sleep(128);
		}
	}
	if (id == 1) {
		sem_destroy(&empty);
		sem_destroy(&full);
		sem_destroy(&mutex);
	}
	exit();
}

void philosopher(int num) {
	int flag = 0;
	sem_t sem[5], access;
	sem_init(&access, 4);
	for (int i = 0; i < num; ++i)
		sem_init(&sem[i], 1);
	for (int i = 0; i < num; ++i) {
		if (flag == 0)
			flag = fork();
		else if (flag > 0) 
			break;
	}
	int id = get_pid();
	while (1) {
		printf("Philosopher %d: before eat think\n", id);
		sleep(128);
		sem_wait(&access);
		sem_wait(&sem[id - 1]);
		sem_wait(&sem[id % (num + 1)]);
		sleep(128);
		printf("Philosopher %d: eat\n", id);
		sleep(128);
		sem_post(&access);
		sem_post(&sem[id % (num + 1)]);
		sem_post(&sem[id - 1]);
		sleep(128);
		printf("Philosopher %d: after eat think\n", id);
		sleep(128);
		
	}
	sem_destroy(&access);
	for (int i = 0; i < num ; ++i)
		sem_destroy(&sem[i]);
	exit();
}

void reader_and_writer(int num) {
	int flag = 0;
	sem_t rw, mutex;
	sem_init(&rw, 1);
	sem_init(&mutex, 1);
	for (int i = 0; i < num; ++i) {
		if (flag == 0)
			flag = fork();
		else if (flag > 0)
			break;
	}
	int id = get_pid();
	if (id == 4 || id == 5 || id == 6) {
		while (1) {
			sem_wait(&mutex);
			sleep(128);
			add_count();
			if (get_count() == 1)
				sem_wait(&rw);
			sem_post(&mutex);
			printf("Reader %d: start read, total %d reader\n", id - 3, get_count());
			sleep(128);
			sem_wait(&mutex);
			sleep(128);
			sub_count();
			if (get_count() == 0)
				sem_post(&rw);
			sem_post(&mutex);
			printf("Reader %d: end read, total %d reader\n", id - 3, get_count());
			sleep(128);
		}
	}
	else {
		while (1) {
			sem_wait(&rw);
			printf("Writer %d: start write\n", id);
			sleep(128);
			sem_post(&rw);
			printf("Writer %d: end write\n", id);
			sleep(128);
		}
	}
	sem_destroy(&rw);
	sem_destroy(&mutex);
}

int uEntry(void)
{
	// For lab4.1
	int dec = 0;
	int hex = 0;
	char str[6];
	char cha = 0;
	int ret = 0;
	while (1)
	{
		printf("Input:\" Test %%c Test %%6s %%d %%x\"\n");
		ret = scanf(" Test %c Test %6s %d %x", &cha, str, &dec, &hex);
		printf("Ret: %d; %c, %s, %d, %x.\n", ret, cha, str, dec, hex);
		if (ret == 4)
			break;
	}

	// For lab4.2
	int i = 4;

	sem_t sem;
	printf("Father Process: Semaphore Initializing.\n");
	ret = sem_init(&sem, 2);
	if (ret == -1)
	{
		printf("Father Process: Semaphore Initializing Failed.\n");
		exit();
	}

	ret = fork();
	if (ret == 0)
	{
		while (i != 0)
		{
			i--;
			printf("Child Process: Semaphore Waiting.\n");
			sem_wait(&sem);
			printf("Child Process: In Critical Area.\n");
		}
		printf("Child Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}
	else if (ret != -1)
	{
		while (i != 0)
		{
			i--;
			printf("Father Process: Sleeping.\n");
			sleep(128);
			printf("Father Process: Semaphore Posting.\n");
			sem_post(&sem);
		}
		printf("Father Process: Semaphore Destroying.\n");
		sem_destroy(&sem);
		exit();
	}

	// For lab4.3
	produce_and_consume(4);
	philosopher(4);
	reader_and_writer(5);
	return 0;
}
