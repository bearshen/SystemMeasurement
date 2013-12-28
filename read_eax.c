#include <sys/time.h>
#include <time.h>
#include <stdio.h>

#define LOOPS 2000

typedef unsigned long long tick;
static __inline__ tick gettick (void) {
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((tick)a) | (((tick)d) << 32));
}

int main(void) {
    tick ts, te; /* tick start, tick end */
    /* start the clock */
    ts = gettick();

    /* do something do something big do something extraordinary do something for the entire humanity */
    volatile int i;
    for (i = 0; i < LOOPS; ++i) {
        __asm__ __volatile__("movl %%ecx, %%eax;":::"%ecx"); 
    }
    
    /* end the clock */
    te = gettick();
    printf ("delta t = %llu\n", te-ts);
}
