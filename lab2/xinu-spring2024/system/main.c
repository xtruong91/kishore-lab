// system/main.c

#include <xinu.h>

void child_process(void)
{
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

void test_usage_response_time(void);
void test_dynamic_priority_scheduling(void);
void test_bonus_problem(void);

process	main(void)
{

#ifdef XINUDEBUG
	kprintf("Test process executing main(): PID = %d\n", getpid());
#endif

	test_usage_response_time();

	test_dynamic_priority_scheduling();
	return OK;
}

void test_usage_response_time(void)
{
	// check create() function
	pid32 childpid = create(child_process, 1024, 30, "child-process-dbg", 0);
	resume(childpid);
	/*
	* Desription: Test and assess whether both clkcounterms and 
	* clktime keep the same time using test code in main().
	*
	* clkcounterms is updated every msec, clktime is updated each one second
	* so clktime = clkcounterms/1000
	*/
	int num_iterations = 50;
	pid32 pid = getpid();
	
	while (num_iterations--)
	{
#ifdef XINUDEBUG	
		kprintf("[%d %d]",clktime,clkcounterms);
#endif
		intmask mask = disable();
		kprintf("Process: %s, clkcounterms: %u, cpu usage: %u, currcpu: %u, resp time: %u, prresptime: %u, prctxswcount: %u\n",proctab[pid].prname,clkcounterms,proctab[pid].prcpu,currcpu,responsetime(pid),proctab[pid].prresptime,proctab[pid].prctxswcount);
		restore(mask);
	}
	kprintf("Process: %s, clkcounterms: %u, cpu usage: %u, currcpu: %u, resp time: %u, prresptime: %u, prctxswcount: %u\n",proctab[childpid].prname,clkcounterms,proctab[childpid].prcpu,currcpu,responsetime(childpid),proctab[childpid].prresptime,proctab[childpid].prctxswcount);
	kprintf("status : %u\n",proctab[childpid].prstate);

}

static void run_benchmark_A()
{
	//1. runs main() with priority 10
	chprio(getpid(),10);
	// 2.  The process running main() spawns 6 app processes each executing cpubnd().
	resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 3", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 4", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 5", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 6", 0));
}

static void run_benchmark_B()
{
	//1. runs main() with priority 10
	chprio(getpid(),10);
	// 2.  The process running main() spawns 6 app processes each executing iobnd().
	resume(create(iobnd, 1024, 30, "io bound 1", 0));
	resume(create(iobnd, 1024, 30, "io bound 2", 0));
	resume(create(iobnd, 1024, 30, "io bound 3", 0));
	resume(create(iobnd, 1024, 30, "io bound 4", 0));
	resume(create(iobnd, 1024, 30, "io bound 5", 0));
	resume(create(iobnd, 1024, 30, "io bound 6", 0));
}

static void run_benchmark_C()
{
	//1. runs main() with priority 10
	chprio(getpid(),10);
	//2. Let the workload generator main() create 6 app processes, half of them executing cpubnd(), the other half iobnd().
	resume(create(iobnd, 1024, 30, "io bound 1", 0));
	resume(create(iobnd, 1024, 30, "io bound 2", 0));
	resume(create(iobnd, 1024, 30, "io bound 3", 0));

	resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 3", 0));
}

static void run_benchmark_D()
{

	/*the process running main() spawn two CPU-bound child processes, then
	sleep for 3 seconds. After waking up, make the process spawn two additional CPU-bound processes */
	resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	sleepms(3000);
	chprio(getpid(),10);
	resume(create(cpubnd, 1024, 30, "cpu bound 3", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 4", 0));
}

void test_dynamic_priority_scheduling(void)
{
	// run_benchmark_A();
	// run_benchmark_B();
	// run_benchmark_C();
	run_benchmark_D();
}


void test_bonus_problem(void)
{
	//Benchmark using 5 processes where 2 run cpubnd(), 2 iobnd(), and 1 joker().
	resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	resume(create(iobnd, 1024, 30, "cpu iobnd 1", 0));
	resume(create(iobnd, 1024, 30, "cpu iobnd 2", 0));
}