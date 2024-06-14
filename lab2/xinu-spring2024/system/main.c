// system/main.c

#include <xinu.h>

void child_process(void);

void test_dynamic_priority_scheduling(); /* For test item 4, Dynamic priority scheduling*/
void test_bonus_problem();


process	main(void)
{
	kprintf("Test process executing main(): PID = %d\n", getpid());

	test_dynamic_priority_scheduling();
	test_bonus_problem();

	return OK;

}

void child_process(void) {
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

void test_cpu_usage_response_time()
{
	// testing that priority is zero
	//resume( create(child_process, 1024, 0, "child-process-dbg", 1) )
	
	//
	pid32 childpid = create(child_process, 1024, 30, "child-process-dbg", 0);
	resume(childpid);

	/* clkcounterms is updated every msec,  clktime is updated every sec */
	int num_iterations = 50;
	intmask mask; 
	pid32 pid = getpid();

	while (num_iterations--)
	{
		//kprintf("[%d %d]",clktime,clkcounterms);
		mask = disable();
		kprintf("Process: %s, clkcounterms: %u, cpu usage: %u, currcpu: %u, resp time: %u, prresptime: %u, prctxswcount: %u\n",proctab[pid].prname,clkcounterms,proctab[pid].prcpu,currcpu,responsetime(pid),proctab[pid].prresptime,proctab[pid].prctxswcount);
		restore(mask);
	}
}

void test_dynamic_priority_scheduling()
{
	// set priority of parent process is 10
	chprio(getpid(),10);
	kprintf("changed priority of process main: priority = %d\n", getprio(currpid));	
	int i;
    /* Benchmark A */
	kprintf("Running benchmark A ....\n");
    for (i = 0; i < 6; i++) {
        resume(create(cpubnd, 1024, 20, "cpubnd", 0));
    }

	/* Benchmark B */
	kprintf("Running benchmark B ....\n");
    for (i = 0; i < 6; i++) {
        resume(create(iobnd, 1024, 20, "iobnd", 0));
    }
	
	/* Benchmark C */
	kprintf("Running benchmark C ....\n");
    for (i = 0; i < 3; i++) {
        resume(create(cpubnd, 1024, 20, "cpubnd", 0));
        resume(create(iobnd, 1024, 20, "iobnd", 0));
    }

	/* Benchmark D */
	kprintf("Running benchmark D ....\n");
    for (i = 0; i < 2; i++) {
        resume(create(cpubnd, 1024, 20, "cpubnd", 0));
    }
    sleepms(3000);
    for (i = 0; i < 2; i++) {
        resume(create(cpubnd, 1024, 20, "cpubndD", 0));
    }
}

void test_bonus_problem()
{
	int i;
    /* Benchmark E */
	kprintf("Running bonus problem ....\n");
    for (i = 0; i < 2; i++) {
        resume(create(cpubnd, 1024, 10, "cpubndE", 0));
        resume(create(iobnd, 1024, 10, "iobndE", 0));
    }
    resume(create(joker, 1024, 10, "joker", 0));
    //sleepms(8000);
}

