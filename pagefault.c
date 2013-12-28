#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

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

typedef unsigned long long tick;
static __inline__ tick gettick (void) {
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((tick)a) | (((tick)d) << 32));
}

int main(int argc, char* argv[]) {
    system ("fallocate -l 1G test.img");
    int pagesize = getpagesize();
    int fd = open("test.img", 0);
    int sum = 0;
    struct stat sbuf;
    fstat(fd, &sbuf);
    int size = sbuf.st_size;
    size -= size % pagesize;
    int npages = size / pagesize;
    char* where;
    int *pages = rpermute(npages);
    int i = 0;
    for (i = 0; i < npages; ++i) {
        pages[i] *= pagesize;
    }
    where = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
    if (msync(where, size, MS_INVALIDATE) != 0) {
        perror("msync");
        exit(1);
    }
    int iter = 10;
    tick store[10];
    tick te, ts;
    while (iter-- > 0) {
        ts = gettick();
        for (i = 0; i < npages; ++i) {
            sum += *(where + pages[i]);
            *pages[i] = *pages[i] + 1;
        }
        te = gettick();
        store[10-iter-1] = (te-ts) / npages;
        munmap(where, size);
        where = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
        if (msync(where, size, MS_INVALIDATE) != 0) {
            perror("msync");
            exit(1);
        }
    }
    printf ("delta t = %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu\n", store[0], store[1], store[2], store[3], store[4], store[5], store[6], store[7], store[8], store[9]);
    munmap (where, size);
    close(fd);
    system ("rm test.img");
    return sum;
}
