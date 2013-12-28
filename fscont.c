#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <unistd.h>
#include <linux/ioctl.h>
#include <sys/file.h>


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
    int exp;
    if (argc <= 1) {
        printf ("usage: test utility for CSE221 project.\n");
        exit(0);
    }
    else {
        exp = atoi (argv[1]);
        if (exp <= 0) {
            printf ("Invalid arguments.\n");
            exit(0);
        }
    }
    /* do something do something big do something extraordinary do something for the entire humanity */
    char device[] = "/dev/sdb";
    char output[200];
    /* char device[] = "/dev/sdb"; */
    tick ts, te;
    int i = 0;
    char* buffer = (char *) malloc (sizeof(char) * BUFFER_SIZE);
    /* read twice, this time for course reasons cannot use system commands */
    int pf = open(device, O_RDONLY);
    // printf ("pf = %d\n", pf);
    long long size_in_bytes;
    ioctl(pf, BLKGETSIZE64, &size_in_bytes);
    // printf ("sz = %ld\n", size_in_bytes);
    long long n = size_in_bytes / BUFFER_SIZE;
    // printf ("n = %lld\n", n);
    long long *seq = rpermute(n-1);
    unsigned long long timelapse = 0;
    long long idx = 0;
    while (i < exp) {
        i++;
        pid_t pid = fork();
        if (pid == 0) {
            idx = idx * 2;
        } else {
            idx = idx * 2 + 1;
        }
        if (exp != i) continue;
        ts = gettick();
        lseek (pf, seq[idx] * BUFFER_SIZE, SEEK_SET); 
        long long cnt = read (pf, buffer, BUFFER_SIZE);
        te = gettick();
        sprintf (output, "echo \"idx = %lld\ttime = %llu cycles\" >> %s",
                idx , 
                te - ts, 
                argv[2]);
        system (output);
    }
    free (buffer);
    free (seq);
    return 0;

}
