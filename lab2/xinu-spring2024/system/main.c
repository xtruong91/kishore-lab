// system/main.c

#include <xinu.h>

void child_process(void){
	int num_iterations = 50;
	pid32 pid = getpid();
	while (num_iterations--)
	{
		intmask mask = disable();
		kprintf("Process: %s, clkcounterms: %u, cpu usage: %u, currcpu: %u, resp time: %u, prresptime: %u, prctxswcount: %u\n",proctab[pid].prname,clkcounterms,proctab[pid].prcpu,currcpu,responsetime(pid),proctab[pid].prresptime,proctab[pid].prctxswcount);
		restore(mask);
	}
	sleepms(2000);

	
	kprintf("Process: %s, clkcounterms: %u, cpu usage: %u, currcpu: %u, resp time: %u, prresptime: %u, prctxswcount: %u\n",proctab[pid].prname,clkcounterms,proctab[pid].prcpu,currcpu,responsetime(pid),proctab[pid].prresptime,proctab[pid].prctxswcount);
	kprintf("status : %u\n",proctab[pid].prstate);
}

process	main(void)
{
	kprintf("Test process executing main(): PID = %d\n", getpid());

	// // check create() function
	// pid32 childpid = create(child_process, 1024, 30, "child-process-dbg", 0);
	// resume(childpid);
	
	// // clkcounterms is updated every msec and
	// // clktime is updated every sec
	// // so clktime = clkcounterms/1000
	// int num_iterations = 50;
	// pid32 pid = getpid();
	
	// while (num_iterations--)
	// {

	// 	//kprintf("[%d %d]",clktime,clkcounterms);
	// 	intmask mask = disable();
	// 	kprintf("Process: %s, clkcounterms: %u, cpu usage: %u, currcpu: %u, resp time: %u, prresptime: %u, prctxswcount: %u\n",proctab[pid].prname,clkcounterms,proctab[pid].prcpu,currcpu,responsetime(pid),proctab[pid].prresptime,proctab[pid].prctxswcount);
	// 	restore(mask);
	// }

	
	// kprintf("Process: %s, clkcounterms: %u, cpu usage: %u, currcpu: %u, resp time: %u, prresptime: %u, prctxswcount: %u\n",proctab[childpid].prname,clkcounterms,proctab[childpid].prcpu,currcpu,responsetime(childpid),proctab[childpid].prresptime,proctab[childpid].prctxswcount);
	// kprintf("status : %u\n",proctab[childpid].prstate);

	chprio(getpid(),10);


	// benchmark A
	// resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	// resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	// resume(create(cpubnd, 1024, 30, "cpu bound 3", 0));
	// resume(create(cpubnd, 1024, 30, "cpu bound 4", 0));
	// resume(create(cpubnd, 1024, 30, "cpu bound 5", 0));
	// resume(create(cpubnd, 1024, 30, "cpu bound 6", 0));

	// benchmark B
	// resume(create(iobnd, 1024, 30, "io bound 1", 0));
	// resume(create(iobnd, 1024, 30, "io bound 2", 0));
	// resume(create(iobnd, 1024, 30, "io bound 3", 0));
	// resume(create(iobnd, 1024, 30, "io bound 4", 0));
	// resume(create(iobnd, 1024, 30, "io bound 5", 0));
	// resume(create(iobnd, 1024, 30, "io bound 6", 0));

	// benchmark C
	// resume(create(iobnd, 1024, 30, "io bound 1", 0));
	// resume(create(iobnd, 1024, 30, "io bound 2", 0));
	// resume(create(iobnd, 1024, 30, "io bound 3", 0));
	// resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	// resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	// resume(create(cpubnd, 1024, 30, "cpu bound 3", 0));

	// benchmark D
	resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	sleepms(3000);
	chprio(getpid(),10);
	resume(create(cpubnd, 1024, 30, "cpu bound 3", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 4", 0));
	

	return OK;

}
