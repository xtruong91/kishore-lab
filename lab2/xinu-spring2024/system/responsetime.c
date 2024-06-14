/* system/responsetime.c - response time */

#include<xinu.h>

/*------------------------------------------------------------------------
 *  response time  -  gets process gross cpu usage time
 *------------------------------------------------------------------------
 */

syscall responsetime(pid32 pid) /* Process ID */
{

	intmask	mask;			/* Saved interrupt mask		*/
	mask = disable();
	uint32 responsetime = 0;

	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	struct procent *prptr = &proctab[pid];

	if(prptr->prctxswcount==0){
		/*  a process may have become ready for
		the first time but not current.
		If so, prctxswcount will be 0 and let
		responsetime() return clkcounterms - prbeginready */
		responsetime = clkcounterms - prptr->prbeginready;
		restore(mask);
		return responsetime;
	}

	if(prptr->prstate==PR_READY){
		/* If pid specified in the argument of responsetime()
		is that of a ready process (i.e., resides in readylist)
		then responsetime() will add clkcounterms - prbeginready
		to prresptime (but not update prresptime) and divide the
		resultant value by prctxswcount + 1 */
		responsetime =  ((clkcounterms - prptr->prbeginready) + prptr->prresptime)/(prptr->prctxswcount+1);
		restore(mask);
		return responsetime;
	}
	responsetime = prptr->prresptime/prptr->prctxswcount;
	restore(mask);
	return responsetime;

}
