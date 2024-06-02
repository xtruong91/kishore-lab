/* cpubnd.c - system/cpubnd.c */

#include<xinu.h>
#include <stdint.h>

/*------------------------------------------------------------------------
 *  cpu bound app -  Create a function called cpubnd() in system/cpubnd.c
 *------------------------------------------------------------------------
 */

void cpubnd(void) {
	
	uint32 startTime = clkcounterms;
    uint32 elapsedTime = 0;

	//check if elapsedTime  exceeds the threshold STOPPINGTIME
	while (elapsedTime <= STOPPINGTIME) {
		    // Calculate elapsed time
    	elapsedTime = clkcounterms - startTime;
	}

    // Print benchmark output
	intmask mask = disable();
    kprintf("PID %d: CPU-bound, clkcounterms: %u, CPU usage: %u, Response time: %u ms\n",
            currpid, clkcounterms, proctab[currpid].prcpu, responsetime(currpid));
	restore(mask);
}
