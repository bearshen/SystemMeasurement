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

/* typedef char testvar; */ 
/* typedef int testvar; */
typedef long long testvar;
/* typedef __int128_t testvar; */

int main(int argc, char* argv[]) {

    int maxsize;
    printf ("\n");
    if (argc <= 1) {
        printf ("By adjusting count to control memory accesses (count*100) hit L1 cache, L2 cache, or main memory.\n*** No input count ***\n");
        exit(0);
    }
    else {
        maxsize = atoi (argv[1]);
        if (maxsize <= 0) {
            printf ("Invalid size <= 0.\n");
            exit(0);
        }
    }

    long long maxsize_in_bytes = maxsize * 1024;
    
    testvar tmp;
    testvar* end;
    testvar junk = 1;
    long long stride;
    tick ts, te; /* tick start, tick end */
    tick tstride[10];

    long long size;
    for (size = 2; size <= maxsize; size += 2) {
        for (stride = 1; stride <= 4096; stride *= 2) {
            testvar* array = (testvar*) malloc(size * 1024);
            memset (array, size, sizeof(array));
            testvar* p = array;
            end = &array[size * 1024 / sizeof(testvar)] - stride;
            ts = gettick();
            do {
                *p = *(p+stride);
            } while (++p < end);
            te = gettick();
            printf ("size = %llu\tstride = %llu\tdelta t = %llu\n", size, stride, ( (te-ts) * sizeof(testvar) ) / ((size-stride*sizeof(testvar)) * 1024));
            free(array); 
        }
    }
}
