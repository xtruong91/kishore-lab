/* joker.c - system/joker.c */

#include<xinu.h>

/*------------------------------------------------------------------------
 *  joker app -  manipulate the scheduler to achieve the desired behavior 
 *	(high CPU usage and small response time
 *  Strategy: 
 *   Observe the behavior of other processes (cpubnd() and iobnd()).
 *   Try to adjust the schedule priority dynamically to maximize CPU usage.
 *------------------------------------------------------------------------
 */

void joker(void) {
	
    // adjust priority 
	chprio(getpid(),15);
	// Perform some CPU-bound operations (e.g., arithmetic, loops)
	uint32 startTime = clkcounterms;
    uint32 elapsedTime = 0;

	//Terminate after 8 seconds
	while (elapsedTime <= STOPPINGTIME) {
		// Calculate elapsed time
    	elapsedTime = clkcounterms - startTime;
	}

    // Print benchmark output

	intmask mask = disable();
    kprintf("PID %d: Joker process, clkcounterms: %u, CPU usage: %u, Response time: %u ms\n",
            currpid, clkcounterms, proctab[currpid].prresptime, responsetime(currpid));
	restore(mask);
}
