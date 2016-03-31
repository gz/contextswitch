/*
 * FILE: measureSingle.c
 * DESCRIPTION: Single process program simulating two processes communications
 * OUTPUT:	time1 = Overhead of traversing through array + pipe overhead
 *   
 *              use measureSwitch to get time2
 *              context switch cost = time2 - time1
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/time.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <linux/unistd.h>
#include "util.h"
void showUsage()
{
  fprintf( stderr, "Usage:\nmeasureSingle <options>\n\
    -n   <number>  size of the array to work on (in byte). default 0.\n\
    -s   <number>  access stride size (in byte). default 0\n");
}

void measureSingle(register int array_size, register int stride,  
	register int *p1, register char *msg, register double *f){
    register int i, j, m;

    for ( i=0; i<LOOP; i++) {
        for ( m=0; m<stride; m++)
            for ( j=m; j<array_size; j=j+stride)
                f[j]++;
        write(p1[1], msg, 1);
        read(p1[0], msg, 1);
    }
}

int main(int argc, char *argv[])
{
    int i, j, len, ret, p1[2], stride=0, array_size=0;
    double *f, start_time, time1, min1=LARGE, min2=LARGE;
    char message, ch;
    short round;
    pid_t p = 0;
    unsigned long new_mask = 2;
    struct sched_param sp;

    len = sizeof(new_mask);
#ifdef MULTIPROCESSOR
    ret = sched_setaffinity(p, len, &new_mask);
    if(ret==-1){
	perror("sched_setaffinity 1");
        exit(1);
    }
    sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
    ret=sched_setscheduler(0, SCHED_FIFO, &sp);
    if(ret==-1){
	perror("sched_setscheduler 1");
        exit(1);
    }
#endif

    while ((ch = getopt(argc, argv, "s:n:")) != EOF) {
        switch (ch) {
            case 'n': /* number of doubles in the array */
                array_size=atoi(optarg);
		array_size=array_size/sizeof(double);
                break;
            case 's': 
		stride=atoi(optarg);
		stride=stride/sizeof(double);
                break;
            default:
                fprintf(stderr, "Unknown option character.\n");
		showUsage();
                exit(1);
        }
    }
    if (stride > array_size){
        printf("Warning: stride is bigger than array_size. "
               "Sequential access. \n");
    }

    /* simulate the execution of two pipes on one process */
    /* create a pipe */
    if (pipe (p1) < 0) {
        perror ("create a pipe");
        return -1;
    }
    printf("time1 without context switch: \t");
    fflush(stdout);
    /* run N times and get the minimum */
    for(round=0; round< N; round++){
        flushCache();
        f = (double*) malloc(array_size*sizeof(double));
        if (f==NULL) {
            perror("malloc failed");
            exit (1);
        }
	memset((void *)f, 0x00, array_size*sizeof(double));
        sleep(1);
        start_time = gethrtime_x86();
        measureSingle(array_size, stride, p1, &message, f);
        time1 = gethrtime_x86()-start_time;
        time1 = time1/LOOP*MILLION;
        if(min1 > time1)
            min1 = time1;
        printf("%f\t", time1);
        fflush(stdout);
        memdump(f, array_size*sizeof(double));
        free(f);
        sleep(1);
    }
    printf("\nmeasureSingle: array_size = %lu, stride = %d, min time1 = %.15f\n", 
	array_size*sizeof(double), stride*sizeof(double), min1);
    return 0;
}
