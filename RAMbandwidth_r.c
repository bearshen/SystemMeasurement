#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef unsigned long long tick;
static __inline__ tick gettick (void) {
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((tick)a) | (((tick)d) << 32));
}

typedef char testvar; 
/* typedef int testvar; */
/* typedef long long testvar; */
/* typedef __int128_t testvar; */

int main(int argc, char* argv[]) {
    int input;
    printf ("\n");
    if (argc <= 1) {
        printf ("By adjusting input (MB).\n*** No input count ***\n");
        exit(0);
    }
    else {
        input = atoi (argv[1]);
        if (input <= 0) {
            printf ("Invalid count <= 0.\n");
            exit(0);
        }
    }

    long long size = input*1024*1024;
    long long cnt = size / sizeof (testvar);
    testvar tmp;

    testvar *array = (testvar*)malloc(size);
    memset (array, 42, size);
    
    testvar* p = array;
    testvar* end = array + cnt;
    tick ts, te; /* tick start, tick end */

    /* start the clock */
    ts = gettick();
    do {
        tmp ^= *p;
    } while (++p < end);
    /* end the clock */
    te = gettick();
    printf ("delta t = %llu\nRAM read bandwidth = %f MB/s\n", te-ts, input/((double)(te-ts) / 3e9));
    free (array);
    return (int) tmp;
}
