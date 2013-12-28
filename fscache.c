#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>

#define BUFFER_SIZE 1024
typedef unsigned long long tick;
static __inline__ tick gettick (void) {
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((tick)a) | (((tick)d) << 32));
}

int main(int argc, char* argv[]) {
    // helper routine that could help the code.
    int loops;
    char cmd_fallocate [200];
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
    tick ts, te; /* tick start, tick end */


    /* do something do something big do something extraordinary do something for the entire humanity */
    volatile int i;
    for (i = 1; i <= loops; ++i) {
        /* start the clock */
        sprintf (cmd_fallocate, "dd if=/dev/zero of=test.img bs=512M count=%d > /dev/null", i*2);
        /* allocate the file */
        system (cmd_fallocate);
        /* clear out the file cache, su needed */
        system ("sync ; sh -c 'echo 3 > /proc/sys/vm/drop_caches'");
        /* read once */
        system ("cat test.img > /dev/null");
        FILE * pf; pf = fopen("test.img", "r");
        fseek (pf, 0L, SEEK_END);
        unsigned long long sz = ftell(pf);
        fseek(pf, 0L, SEEK_SET);
        char* buffer = (char *) malloc (sizeof(char) * BUFFER_SIZE);
        ts = gettick();
        /* read twice, this time for course reasons cannot use system commands */
        while (!feof(pf)) {
            fread (buffer, sizeof(char), BUFFER_SIZE, pf);
        }
        te = gettick();
        fclose(pf);
        fsync();
        /* drop the file */
        system ("unlink test.img");
        free (buffer);
        printf ("filesize = %dG\tt = %llu\n", i, (te-ts)/i);
    }
    /* end the clock */
}
