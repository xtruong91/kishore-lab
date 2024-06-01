/* system/responsetime.c - response time */

#include<xinu.h>

/*------------------------------------------------------------------------
 *  response time  -  gets process gross cpu usage time
 *------------------------------------------------------------------------
 */

syscall responsetime(pid32 pid) /* Process ID */
{

	intmask mask; 		/*Saved intterupt mask*/
	uint32 grosscpu;	/* process gross cpu time to return*/
	uint32  tmp = 0;

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	struct procent *prptr = &proctab[pid];

	/*For 3.2, Consider border cases, such as when a process is context-switched in for the first time (i.e., prctxswcount is 0*/
	if(prptr->prctxswcount==0) {
		grosscpu = clkcounterms - prptr->prbeginready;
		restore(mask);	
		return grosscpu;
	}

	/* if the specified process ID to a ready process */
	if(prptr->prstate==PR_READY) {
		/*1. Add clkcounterms - prbeginready to prresptime (without updating prresptime) */
		tmp = clkcounterms-prptr->prbeginready + prptr->prresptime;
		/* 2. Divide the resultant value by prctxswcount + 1*/
		grosscpu = tmp /(prptr->prctxswcount + 1);
		restore(mask);
		/*3. Return the average response time (rounded to an integer, in milliseconds)*/
		return grosscpu;
	} else {
		// return the avarage response time
		grosscpu = prptr->prresptime/prptr->prctxswcount;
		restore(mask);
		return grosscpu;
	}
}
