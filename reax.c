#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>

typedef unsigned long long tick;
static __inline__ tick gettick (void) {
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((tick)a) | (((tick)d) << 32));
}

int main(int argc, char* argv[]) {
    // helper routine that could help the code.
    int loops;
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

    // auxillary functions that help the test
    volatile int a[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    tick ts, te; /* tick start, tick end */
    /* start the clock */
    ts = gettick();

    /* do something do something big do something extraordinary do something for the entire humanity */
    volatile int i;
    for (i = 0; i < loops; ++i) {
        __asm__ __volatile__("movl %eax, %eax");
    }
    
    /* end the clock */
    te = gettick();
    printf ("delta t = %llu\n", te-ts);
}
