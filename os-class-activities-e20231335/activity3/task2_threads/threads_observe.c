#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4

static void *worker(void *arg)
{
    int id = *(int *)arg;

    printf("Thread %d: sleeping for 60 seconds...\n", id);
    sleep(60);
    printf("Thread %d: done.\n", id);

    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    printf("PID: %d — observe the threads now!\n", getpid());
    fflush(stdout);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;

        int error = pthread_create(&threads[i], NULL, worker, &ids[i]);
        if (error != 0) {
            fprintf(stderr, "pthread_create failed: %d\n", error);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return EXIT_SUCCESS;
}
