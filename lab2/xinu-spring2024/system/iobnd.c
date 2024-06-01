/* iobnd.c - system/iobnd.c */

#include<xinu.h>
/*------------------------------------------------------------------------
 *  io bound app -  Create a function called iobnd() in system/iobnd.c
 *------------------------------------------------------------------------
 */

void iobnd(void){
	
	uint32_t startTime = clkcounterms;
    uint32_t elapsedTime;

	//Check if clkcounterms has exceeded STOPPINGTIME
	while (elapsedTime <= STOPPINGTIME)
	{
		// Run an I/O-bound task (e.g., nested loops with sleep)
		for (int i = 0; i < 100; ++i) {
			for (int j = 0; j < 1000; ++j) {
				// Some arbitrary computation
				int result = i * j;
			}
			sleepms(100); // Sleep for 100 milliseconds
		}

		// Calculate elapsed time
    	elapsedTime = clkcounterms - startTime;		
	}

    // Print benchmark output
    kprintf("PID %d: I/O-bound, clkcounterms: %u, CPU usage: %.2f%%, Response time: %u ms\n",
            currpid, clkcounterms, (float)elapsedTime / STOPPINGTIME * 100, elapsedTime);

}
