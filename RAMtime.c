#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

#define ONE    	p = p->next;
#define FIVE	ONE ONE ONE ONE ONE
#define TEN	FIVE FIVE
#define FIFTY	TEN TEN TEN TEN TEN
#define HUNDRED	FIFTY FIFTY
#define FIVEHUNDRED HUNDRED HUNDRED HUNDRED HUNDRED HUNDRED
#define THOUSAND FIVEHUNDRED FIVEHUNDRED

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

struct cell { struct cell *next; };

int main(int argc, char* argv[]) {
    volatile int i;
    tick ts, te; /* tick start, tick end */
    tick store[10];
    int input;
    if (argc <= 1) {
        printf ("By adjusting input (Kword).\n*** No input count ***\n");
        exit(0);
    }
    else {
        input = atoi (argv[1]);
        if (input <= 0) {
            printf ("Invalid Kword <= 0.\n");
            exit(0);
        }
    }
    
    struct cell *ptr = (struct cell* ) malloc ( (input * 1024) * sizeof(struct cell));
    int *l = rpermute(input * 1024);
    // for (i = 0; i < 6 * 1024 * 1024; ++i) {
    //     (ptr+i)->next = ptr + (i+1);
    // }
    for (int i = 0; i < input * 1024; ++i) {
        ( ptr+l[i%(input*1024)] ) -> next = (ptr+l[(i+1)%(input*1024)]);
    }
    free(l);
    printf ("%lu\n", sizeof(struct cell));
    volatile struct cell *p;
    // By adjusting count to control memory accesses hit L1 cache, L2 cache, or main memory
    for (i = 0; i < 10; i++) {
        p = ptr;
	int cnt = input;
        ts = gettick();
	while (cnt -- >0 ) {
	    THOUSAND TEN TEN ONE ONE ONE ONE
        } 
        te = gettick();
        store[i] = te - ts;
    } 
    /* end the clock */
    free (ptr);
    printf ("delta t = %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu\n", store[0], store[1], store[2], store[3], store[4], store[5], store[6], store[7], store[8], store[9]);
    return (int) p;
}
