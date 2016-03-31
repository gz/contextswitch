/*
 * FILE: util.h
 * DESCRIPTION: Header file for util.c
 */

#define N       3
#define LOOP    10000
#define MILLION 1000000
#define LARGE   100000000
#define FLUSHSIZE	4194304
#define MULTIPROCESSOR	

typedef long long hrtime_t;

/* get the elapsed time (in seconds) since startup */
double gethrtime_x86(void);

/* get the number of CPU cycles since startup */
hrtime_t gethrcycle_x86(void);

/* get the number of CPU cycles per microsecond - from Linux /proc filesystem */
double getMHZ_x86(void);

/* dump a chunk of memory to /dev/null */
void memdump(double *m, int bytes);

/* flush the cache */
void flushCache();

