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
	uint32_t startTime = clkcounterms;
    uint32_t elapsedTime = 0;

	//Terminate after 8 seconds
	while (elapsedTime <= STOPPINGTIME) {
		// Calculate elapsed time
    	elapsedTime = clkcounterms - startTime;
	}

    // Print benchmark output
    kprintf("PID %d: CPU-bound, clkcounterms: %u, CPU usage: %.2f%%, Response time: %u ms\n",
            currpid, clkcounterms, (float)elapsedTime / STOPPINGTIME * 100, elapsedTime);
}
