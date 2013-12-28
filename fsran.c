#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
typedef unsigned long long tick;
static __inline__ tick gettick (void) {
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((tick)a) | (((tick)d) << 32));
}

long long * rpermute(int n) {
    long long *a = malloc(n*sizeof(long long));
    long long k;
    for (k = 0; k < n; k++)
	a[k] = k;
    for (k = n-1; k > 0; k--) {
	long long j = rand() % (k+1);
	long long temp = a[j];
	a[j] = a[k];
	a[k] = temp;
    }
    return a;
}


int main(int argc, char* argv[]) {
    printf ("usage: test utility for CSE221 project.\n");
    /* do something do something big do something extraordinary do something for the entire humanity */
    char device[] = "/dev/sdb";
    /* char device[] = "/dev/sdb"; */
    tick ts, te;
    int i = 0;
    char* buffer = (char *) malloc (sizeof(char) * BUFFER_SIZE);
    ts = gettick();
    /* read twice, this time for course reasons cannot use system commands */
    int pf = open(device, O_RDONLY);
    // printf ("pf = %d\n", pf);
    long long size_in_bytes;
    ioctl(pf, BLKGETSIZE64, &size_in_bytes);
    // printf ("sz = %ld\n", size_in_bytes);
    long long n = size_in_bytes / BUFFER_SIZE;
    // printf ("n = %d\n", n);
    long long *seq = rpermute(n);
    unsigned long long timelapse = 0;
    while (i < 1000) {
        ts = gettick();
        lseek (pf, seq[i] * BUFFER_SIZE, SEEK_SET); 
        long long cnt = read (pf, buffer, BUFFER_SIZE);
        te = gettick();
        
        // when large enough, record the read speed.
        printf ("offset = %f MB\ttime = %llu cycles\n",
            (double)seq[i] * 4096.0 / 1048576.0 , 
            te - ts);
        i++;
    }
    close(pf);
    free (buffer);
    free (seq);
}
