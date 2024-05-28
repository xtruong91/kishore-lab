#include<xinu.h>

void iobnd(void){
	
	while (clkcounterms<=STOPPINGTIME)
	{
		int n = 500;
		/* Try different values for the bound of
		the inner for-loop such that it consumes
		several milliseconds of CPU time. It should
		not exceed 10 msec but otherwise is not important. */
		// TODO : RECHECK n
		int i;
		for(i=0;i<n;i++){
			/*
				The inner for-loop can contain arithmetic operations (even a nested for-loop) to help consume CPU time not exceeding 10 msec. Inspect the value of clkcounterms before and after the for-loop to calibrate the bound.
			*/
			int x = 10;
			int y = 20;
			uint32 z = x+y;
			int j;
			for(j=0;j<10;j++){
				z+=x;
				z+=y;
				z/=x;
			}
		}

		sleepms(100);
		
	}

	intmask mask = disable();
	kprintf(" PID : %d, I/O-bound, clkcounterms: %u, CPU usage: %u, response time: %d\n" , getpid(),clkcounterms,proctab[getpid()].prcpu, responsetime(getpid()) );
	restore(mask);

}
