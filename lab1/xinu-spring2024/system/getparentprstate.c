/* getparentprstate.c - getparentprstate, newpid */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getparentprstate  -  get state of parent process
 *------------------------------------------------------------------------
 */
int	getparentprstate(
	  pid32		pid	/* process id for whose parent status we want to know	*/
	)
{
	/**
	 * check that the specified PID falls within the valid
	 * range of PIDs 0, ..., NPROC-1 and the PID is assigned
	 * to any one of the processes. If the argument passed is
	 * invalid, getparentprstate() returns SYSERR
	*/
	if(pid<0 || pid>=NPROC || proctab[pid].prstate==PR_FREE){
		return SYSERR;
	}

	pid32 prparent = proctab[pid].prparent;

	if(prparent<0 || prparent>=NPROC){
		return SYSERR;
	}
	
	return proctab[prparent].prstate;
}
