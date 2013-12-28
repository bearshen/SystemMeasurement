#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096
typedef unsigned long long tick;
static __inline__ tick gettick (void) {
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((tick)a) | (((tick)d) << 32));
}

int * rpermute(int n) {
    int *a = malloc(n*sizeof(int));
    int k;
    for (k = 0; k < n; k++)
	a[k] = k;
    for (k = n-1; k > 0; k--) {
	int j = rand() % (k+1);
	int temp = a[j];
	a[j] = a[k];
	a[k] = temp;
    }
    return a;
}

int main(int argc, char* argv[]) {
    // printf ("usage: test utility for CSE221 project.\n");
    /* do something do something big do something extraordinary do something for the entire humanity */
    char device[] = "/dev/sda";
    /* char device[] = "/dev/sdb"; */
    tick ts, te;
    int i = 0;
    char* buffer = (char *) malloc (sizeof(char) * BUFFER_SIZE);
    ts = gettick();
    /* read twice, this time for course reasons cannot use system commands */
    int pf = open(device, O_RDONLY); int cnt = BUFFER_SIZE; int last_i = 0;
    unsigned long long timelapse = 0;
    while (cnt > 0) {
        ts = gettick();
        cnt = read (pf, buffer, BUFFER_SIZE);
        te = gettick();
	timelapse = timelapse + (te - ts);
        i++;
        // when large enough, record the read speed.
        if (timelapse >= 3000000000) {
            printf ("%f\t%f\n",
                (double)last_i * 4096.0 / 1048576.0 , 
                (double)(i-last_i) * 4096.0 / 1048576.0 / (timelapse / 3e9));
	    last_i = i;
            timelapse = 0;
        }
    }
    close(pf);
    free (buffer);
}
