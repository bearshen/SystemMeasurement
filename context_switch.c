#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
typedef unsigned long long tick;

int loops;
int pipefd[2];
pthread_t threadid;

static __inline__ tick gettick (void) {
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((tick)a) | (((tick)d) << 32));
}

void * thread1 (void * unused) {
    // continuously write current timestamp and switches out.
    tick ts;
    volatile int j;
    for (j = 0; j < loops; ++j) {
        ts = gettick();
        write(pipefd[1], &ts, sizeof(ts));
        usleep(2000);
    }
}

int main(int argc, char* argv[]) {
    // helper routine that could help the code.
    if (argc <= 1) {
        printf ("usage: test utility for CSE221 project.\n");
        exit(0);
    }
    else {
        loops = atoi (argv[1]);
        if (loops <= 0) {
            printf ("Invalid arguments.\n");
            exit(0);
        }
    }

    if (pipe(pipefd) == 0) {
    } else {
        exit(0);
    }

    pthread_create(&threadid, (pthread_attr_t *)NULL, thread1, NULL);
    volatile int i;
    for (i = 0; i < loops; ++i) {
        // read from the other thread.
        tick ts, te;
        read (pipefd[0], &ts, sizeof(ts));
        te = gettick();
        printf("delta t=%llu\n", te-ts);
    }
}
