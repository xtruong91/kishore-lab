/* iobnd.c - system/iobnd.c */

#include<xinu.h>
#include <stdint.h>

/*------------------------------------------------------------------------
 *  io bound app -  Create a function called iobnd() in system/iobnd.c
 *------------------------------------------------------------------------
 */

void iobnd(void){
	
	uint32 startTime = clkcounterms;
    uint32 elapsedTime = 0;
	int i, j;

	//Check if clkcounterms has exceeded STOPPINGTIME
	while (elapsedTime <= STOPPINGTIME)
	{
		// Run an I/O-bound task (e.g., nested loops with sleep)
		for (i = 0; i < 100; ++i) {
			for (j = 0; j < 1000; ++j) {
				// Some arbitrary computation
				int result = i * j;
			}
			sleepms(100); // Sleep for 100 milliseconds
		}

		// Calculate elapsed time
    	elapsedTime = clkcounterms - startTime;		
	}

    // Print benchmark output
	intmask mask = disable();
    kprintf("PID %d: I/O-bound, clkcounterms: %u, CPU usage: %u, Response time: %u ms\n",
            currpid, clkcounterms, proctab[currpid].prresptime , responsetime(currpid));
	restore(mask);

}
