#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
typedef unsigned long long tick;

int loops;
pthread_t threadid;

static __inline__ tick gettick (void) {
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((tick)a) | (((tick)d) << 32));
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
    tick ts, te;    
    // read from the other thread.
    ts = gettick();
    pid_t pid = fork();
    if (pid == 0) {
        te = gettick();
        printf("delta t = %llu\n", te-ts);
    }
}
