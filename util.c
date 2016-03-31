/*
 * FILE: util.c
 * DESCRIPTION: Utility program for context switch cost measurement
 *              including a high-resolution timer on x86 architecture
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "util.h"

/* get the elapsed time (in seconds) since startup */
double gethrtime_x86(void)
{
    static double CPU_MHZ=0;
    if (CPU_MHZ==0) CPU_MHZ=getMHZ_x86();
    return (gethrcycle_x86()*0.000001)/CPU_MHZ;
}

/* get the number of CPU cycles since startup */
hrtime_t gethrcycle_x86(void)
{
    unsigned int tmp[2];

    __asm__ ("rdtsc"
	     : "=a" (tmp[1]), "=d" (tmp[0])
	     : "c" (0x10) );
        
    return ( ((hrtime_t)tmp[0] << 32 | tmp[1]) );
}

/* get the number of CPU cycles per microsecond - from Linux /proc filesystem 
 * return<0 on error
 */
double getMHZ_x86(void)
{
    double mhz = -1;
    char line[1024], *s, search_str[] = "cpu MHz";
    FILE *fp; 
    
    /* open proc/cpuinfo */
    if ((fp = fopen("/proc/cpuinfo", "r")) == NULL)
	return -1;

    /* ignore all lines until we reach MHz information */
    while (fgets(line, 1024, fp) != NULL) { 
	if (strstr(line, search_str) != NULL) {
	    /* ignore all characters in line up to : */
	    for (s = line; *s && (*s != ':'); ++s);
	    /* get MHz number */
	    if (*s && (sscanf(s+1, "%lf", &mhz) == 1)) 
		break;
	}
    }

    if (fp!=NULL) fclose(fp);

    return mhz;
}
/* dump a chunk of memory to /dev/null
 */
void memdump(double *m, int bytes)
{
    int fd;

    if ((fd=open("/dev/null",O_WRONLY)) == -1){
	perror("/dev/null open error");
        exit(-1);
    }
    if (write(fd, (void *)m, bytes) == -1){
	perror("/dev/null write error");
        exit(-1);
    }
    if (close(fd) != 0){
	perror("/dev/null close error");
        exit(-1);
    }
}
/* flush the cache
 */
void flushCache()
{
    char *f;
    static char foo=0;
        f = (char*) malloc(FLUSHSIZE);
        if (f==NULL) {
            perror("malloc fails");
            exit (1);
        }
        memset((void *)f, foo++, FLUSHSIZE);
        free(f);
}

