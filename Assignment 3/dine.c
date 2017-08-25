#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

void *simu(void *p);

typedef struct philo 
{
    int num;
    int fail;
    int eatLeft;
    pthread_mutex_t *right;
    pthread_mutex_t *left;
    pthread_t thread;
} Philosopher;

int main(int argc, char * argv[])
{
    int numPhil;
    int eatLeft;
    int i;
    Philosopher *phil;

    numPhil = atoi(argv[1]);
    eatLeft = atoi(argv[2]);
    
    pthread_mutex_t forks[numPhil]; //create threads for number of philosophers

    Philosopher philosophers[numPhil];
    
 
    for (i=0;i<numPhil; i++)
    {
        pthread_mutex_init(&forks[i], NULL);
        phil = &philosophers[i];//fill the struct with information
        phil->left = &forks[i];
        phil->right = &forks[(i+1)%numPhil];
        phil->num=i;
        phil->eatLeft=eatLeft;
        phil->fail = pthread_create( &phil->thread, NULL, simu, phil); //create the thread
    }

    for(i=0; i<numPhil; i++) 
    {
        phil = &philosophers[i];
        pthread_join( phil->thread, NULL);
    }
    return 0;
}


void *simu(void *p) 
{
    Philosopher *phil = (Philosopher*) p;
    int wrong;
    int attempt;
    pthread_mutex_t *left;
    pthread_mutex_t *right;
    pthread_mutex_t *temp;
    int i;

    for(i=1;i <= phil->eatLeft;++i)
    {
        printf("Philosopher %d thinking\n", phil->num+1);
        sleep(1);
        left = phil->left;
        right = phil->right;
        attempt = 1;

        do 
        {
            wrong = pthread_mutex_lock(left);
            
            if(attempt < 0)
        	{
                wrong = pthread_mutex_lock(right);
            }
            else
            {
            	wrong = pthread_mutex_trylock(right);

            } 

            if (wrong) 
            {
                pthread_mutex_unlock(left);
                temp = left;
                left = right;
                right = temp;
                attempt--;
            }

        } while (wrong);
 
        if (!wrong) 
        {
            printf("Philosopher %d is eating\n", phil->num+1);
            if(i == phil->eatLeft)
            {
                printf("Philosopher %d has eaten\n",phil->num+1);
            }
            sleep(1);
            pthread_mutex_unlock(left);
            pthread_mutex_unlock(right);
        }
    }
    return NULL;
}