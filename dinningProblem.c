#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define PHILOS 5
#define DELAY 5000
#define FOOD 15
#define true 1
#define false 0

void *philosopher (void *id);
int food_on_table ();

//Mutexes
pthread_mutex_t chopstick[PHILOS];
pthread_mutex_t food_lock;

//Threads
pthread_t philo[PHILOS];
int sleep_seconds = 0;


int main (int argn, char **argv)
{
	long i;

	if (argn == 2)
	sleep_seconds = atoi (argv[1]);
	//Inicia mutexes
	pthread_mutex_init (&food_lock, NULL);
	for (i = 0; i < PHILOS; i++)
		pthread_mutex_init (&chopstick[i], NULL);
	//Cria threads
	for (i = 0; i < PHILOS; i++)
		pthread_create (&philo[i], NULL, philosopher, (void *)i);
	//Join threads	
	for (i = 0; i < PHILOS; i++)
		pthread_join (philo[i], NULL);
	return 0;
}

//Thread
void * philosopher (void *num)
{
	long id = (long)num;
	long i, left_chopstick, right_chopstick, f;
	
	left_chopstick = id;
	right_chopstick = (id + 1)%PHILOS;
	while (f = food_on_table ()) {
		if(id % 2 == 0){
			pthread_mutex_lock (&chopstick[left_chopstick]);
			printf ("Philosopher %ld: got left chopstick %ld\n", id,  left_chopstick);
			pthread_mutex_lock (&chopstick[right_chopstick]);
			printf ("Philosopher %ld: got right chopstick %ld\n", id,  right_chopstick);
		}
		else{
			pthread_mutex_lock (&chopstick[right_chopstick]);
			printf ("Philosopher %ld: got right chopstick %ld\n", id,  right_chopstick);
			pthread_mutex_lock (&chopstick[left_chopstick]);
			printf ("Philosopher %ld: got left chopstick %ld\n", id,  left_chopstick);
		}
		printf ("Philosopher %ld: eating -- food %ld.\n", id, f);
		pthread_mutex_unlock (&chopstick[left_chopstick]);
		pthread_mutex_unlock (&chopstick[right_chopstick]);
		// int thinking_time=rand()%10;
		// printf ("Philosopher %ld is done eating and is now thinking for %d secs.\n", id, thinking_time);
		// sleep (thinking_time);
	}
	return (NULL);
}

int food_on_table ()
{
    static int food = FOOD;
    int myfood;

    pthread_mutex_lock (&food_lock);
    if (food > 0) {
        food--;
    }
    myfood = food;
    pthread_mutex_unlock (&food_lock);
    return myfood;
}


