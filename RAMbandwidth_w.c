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



/* testing against four types of read pattern: 64 bit, 128 bit, 256 bit */

typedef struct {
    long long d[4];
} __int256_t;

typedef struct {
    long long d[8];
} __int512_t;

typedef struct {
    long long d[16];
} __int1024_t;

typedef struct {
    __int128_t d[32];
} __int2048_t;

typedef char testvar;

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
    volatile testvar tmp;

    testvar *array = (testvar*)malloc(size);
    memset (array, 0xff, size);
    
    testvar* p = array;
    testvar* end = array + cnt;
    tick ts, te; /* tick start, tick end */

    /* start the clock */
    ts = gettick();
    memset (array, 0, size);
    /* end the clock */
    te = gettick();
    printf ("delta t = %llu\nRAM read bandwidth = %f MB/s\n", te-ts, input/((double)(te-ts) / 3e9));
    free (array);
}
