/* wakeup.c - wakeup */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wakeup  -  Called by clock interrupt handler to awaken processes
 *------------------------------------------------------------------------
 */
void	wakeup(void)
{
	/* Awaken all processes that have no more time to sleep */
	pri16 old_priority; // previous prioriy 
	pri16 new_priority; // new priority
	pid32 pid; //sleep process ID

	resched_cntl(DEFER_START);
	while (nonempty(sleepq) && (firstkey(sleepq) <= 0)) 
	{
		pid = dequeue(sleepq);
		old_priority =  proctab[pid].prprio;

		if(old_priority>=1 && old_priority<=10)
		{
			new_priority = dynprio[old_priority].ts_slpret;
			chprio(pid,new_priority); // since they were sleeping , they are IO bound processes
			proctab[pid].time_slice = dynprio[new_priority].ts_quantum;
		}

		ready(pid);
	}

	resched_cntl(DEFER_STOP);
	return;
}
