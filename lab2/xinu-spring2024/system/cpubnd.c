#include<xinu.h>

void cpubnd(void) {
	
	while (clkcounterms<=STOPPINGTIME)
	{

	}

	intmask mask = disable();
	kprintf(" PID : %d, CPU-bound, clkcounterms: %u, CPU usage: %u, response time: %d\n" , getpid(),clkcounterms,proctab[getpid()].prcpu, responsetime(getpid()) );
	restore(mask);

}
