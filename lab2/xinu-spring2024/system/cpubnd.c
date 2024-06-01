/* cpubnd.c - system/cpubnd.c */

#include<xinu.h>

/*------------------------------------------------------------------------
 *  cpu bound app -  Create a function called cpubnd() in system/cpubnd.c
 *------------------------------------------------------------------------
 */

void cpubnd(void) {
	
	uint32_t startTime = clkcounterms;
    uint32_t elapsedTime = 0;

	//check if elapsedTime  exceeds the threshold STOPPINGTIME
	while (elapsedTime <= STOPPINGTIME) {
		    // Calculate elapsed time
    	elapsedTime = clkcounterms - startTime;
	}

    // Print benchmark output
    kprintf("PID %d: CPU-bound, clkcounterms: %u, CPU usage: %.2f%%, Response time: %u ms\n",
            currpid, clkcounterms, (float)elapsedTime / STOPPINGTIME * 100, elapsedTime);
}
