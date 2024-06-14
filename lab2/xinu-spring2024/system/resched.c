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

	ptold = &proctab[currpid];

#ifdef XINUDEBUG
		if(currpid != 0){
			kprintf("switched out PID %d, state = %d, clk= %u, prcpu=%u, currcpu=%u \r\n" , currpid, ptold->prstate, clkcounterms,ptold->prcpu, currcpu);
		}
#endif

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */

		/*For the currently running process, CPU usage will be the sum of prcpu and a global variable currcpu*/
		 

		if (ptold->prprio > firstkey(readylist)) {
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		ptold->prbeginready = clkcounterms;		
		insert(currpid, readylist, ptold->prprio);		
	}

	ptold->prcpu += currcpu;
	currcpu = 0;

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
#ifdef XINUDEBUG
		if(currpid != 0){
			kprintf("switched in PID %d, state = %d, clk= %u, prcpu=%u, currcpu=%u \r\n" , currpid, ptnew->prstate, clkcounterms,ptnew->prcpu, currcpu);
		}
#endif

	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	
#if (1)
	int32 time_in_ready = clkcounterms - ptnew->prbeginready;
	if(time_in_ready == 0){
		ptnew->prresptime += 1;
	}else{
		ptnew->prresptime += time_in_ready;
	}
	ptnew->prctxswcount ++;
	ptnew->time_slice = dynprio[ptnew->prprio].ts_quantum;
#else // FIXED_PRIO

#endif	
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
