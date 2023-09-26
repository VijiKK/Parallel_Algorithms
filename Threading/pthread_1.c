#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 30
#define MAXVAL 99999999999999999
const int NUM_VALUES = 10;

/* we put this in a struct so that we could have multiple barriers throughout
 * a program if needed.. */
struct barrier
{
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int num_threads;
    int count;
};

void barrier_init(struct barrier* b, int nt)
{
    /* b->lock = PTHREAD_MUTEX_INITIALIZER; */
    /* b->cond = PTHREAD_COND_INITIALIZER; */

    /* we cannot assign an INITIALIZER to a struct field, so we initialize it
     * directly */
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init(&b->cond, NULL);


    b->count=0;
    b->num_threads=nt;
}

void barrier_destroy(struct barrier* b)
{
    pthread_mutex_destroy(&b->lock);
    pthread_cond_destroy(&b->cond);
}


void barrier(struct barrier* b)
{
    pthread_mutex_lock(&b->lock);

    b->count++;

    if (b->count == b->num_threads || b->count == NUM_VALUES)
    {
        /* wake everyone up -- we've all reached the barrier */
        pthread_cond_broadcast(&b->cond);
    }
    else
    {
        while (b->count  < b->num_threads)
        {
            /* wait for everyone else... */
            pthread_cond_wait(&b->cond, &b->lock);
        }
    }

    pthread_mutex_unlock(&b->lock);
}


/* a structure like this could be used to pass complex parameters to each
   ,* thread ... */

struct thread_args
{
    int rank;
    struct barrier* barrier;
};


void* start(void* x)
{
    struct thread_args* args = (struct thread_args*) x;

    //if(args->barrier->count == NUM_VALUES) return NULL;

    unsigned long int randomNumber;

    arc4random_buf(&randomNumber, sizeof(randomNumber));
    randomNumber =  randomNumber % MAXVAL;
    printf("%lu ", randomNumber);

    printf("%d about to barrier\n", args->rank);
    barrier(args->barrier);

    printf("%d done with barrier\n", args->rank);

    return NULL;    
}


int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    struct thread_args args[NUM_THREADS];

    struct barrier sb;
    barrier_init(&sb, NUM_THREADS);

    for (int i=0; i<NUM_THREADS; i++)
    {
        /* set up thread args */
        args[i].rank = i;
        args[i].barrier = &sb;
        
        pthread_create(&threads[i], NULL, start, &args[i]);
    }

    for (int i=0; i<NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    barrier_destroy(&sb);

    return 0;
}