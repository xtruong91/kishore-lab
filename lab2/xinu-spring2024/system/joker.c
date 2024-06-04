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
	uint32 i;

	//Terminate after 8 seconds
	while (elapsedTime < STOPPINGTIME) {
		// Calculate elapsed time
		for (i = 0; i < 10000; i++) {
            /* Consume some CPU time */
        }
        if (clkcounterms % 2 == 0) {
            /* Occasionally make a blocking call to appear I/O-bound */
            sleepms(100);
        }
    	elapsedTime = clkcounterms - startTime;
	}

    // Print benchmark output
	intmask mask = disable();
    kprintf("PID %d: Joker process, clkcounterms: %u, CPU usage: %u, Response time: %u\n",
            currpid, clkcounterms, proctab[currpid].prcpu, responsetime(currpid));
	restore(mask);
}
