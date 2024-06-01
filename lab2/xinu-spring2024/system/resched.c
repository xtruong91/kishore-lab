/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	/* added for 4.1, handle the context-switched */
	pid32 oldpid = currpid;
	ptold = &proctab[currpid];

#ifndef XINUDEBUG 	
	kprintf("old Process: %s, old state: %u, new Process: %s, new state: %u\n", ptold->prname,ptold->prstate, ptnew->prname,ptnew->prstate);
#endif

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		ptold->prbeginready = clkcounterms;
		insert(currpid, readylist, ptold->prprio);
		
	}

	// Added for 3.2 CPU Usage will be sum of prcpu and currcpu
	ptold->prcpu += currcpu; // add currcpu to prcpu of process being context switched out
	currcpu = 0; // reset currcpu

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	if(ptnew->prstate == PR_READY){
		int32 diff = clkcounterms - ptnew->prbeginready;
		if(diff<1)diff=1;
		ptnew->prresptime += diff;
		ptnew->prctxswcount ++;
		//kprintf("Context switch newpid: %d, newpidname: %s, oldpid: %d, new_prctxswcount: %u , old_prctxswcount: %u\n",currpid,ptnew->prname,oldpid,ptnew->prctxswcount,ptold->prctxswcount);
	}
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	ptnew->time_slice = dynprio[ptnew->prprio].ts_quantum;
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
