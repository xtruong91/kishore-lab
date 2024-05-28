#include<xinu.h>

syscall responsetime(pid32 pid){

	if (isbadpid(pid)) {
		return SYSERR;
	}

	struct procent *prptr = &proctab[pid];

	if(prptr->prctxswcount==0){
		/*  a process may have become ready for
		the first time but not current.
		If so, prctxswcount will be 0 and let
		responsetime() return clkcounterms - prbeginready */
		return clkcounterms - prptr->prbeginready;
	}


	if(prptr->prstate==PR_READY){
		/* If pid specified in the argument of responsetime()
		is that of a ready process (i.e., resides in readylist)
		then responsetime() will add clkcounterms - prbeginready
		to prresptime (but not update prresptime) and divide the
		resultant value by prctxswcount + 1 */
		return (clkcounterms-prptr->prbeginready + prptr->prresptime)/(prptr->prctxswcount+1);
	}

	return prptr->prresptime/prptr->prctxswcount;
}
