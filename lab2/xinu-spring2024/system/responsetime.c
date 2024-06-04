/* system/responsetime.c - response time */

#include<xinu.h>

/*------------------------------------------------------------------------
 *  response time  -  gets process gross cpu usage time
 *------------------------------------------------------------------------
 */

syscall responsetime(pid32 pid) /* Process ID */
{

	intmask mask; 		/*Saved intterupt mask*/
	uint32  total_resptime = 0;

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	
	struct procent *prptr = &proctab[pid];
	total_resptime = prptr->prresptime;

	/*For 3.2, Consider border cases, such as when a process is context-switched in for the first time (i.e., prctxswcount is 0*/
	/* if the specified process ID to a ready process */
	if(prptr->prstate==PR_READY) {
		total_resptime += clkcounterms - prptr->prbeginready;
	}
	restore(mask)
	return prptr->prctxswcount == 0 ? total_resptime : total_resptime / prptr->prctxswcount;

}
