// system/main.c

#include <xinu.h>

void child_process(void);

void test_cpu_usage_response_time(); /*For test item 3, Monitoring process CPU usage and response time*/
void test_dynamic_priority_scheduling(); /* For test item 4, Dynamic priority scheduling*/
void test_bonus_problem();

static void run_benchmark_A();
static void run_benchmark_B();
static void run_benchmark_C();
static void run_benchmark_D();


process	main(void)
{
	kprintf("Test process executing main(): PID = %d\n", getpid());



	//test_cpu_usage_response_time();

	//test_dynamic_priority_scheduling();

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
	run_benchmark_A();
	 run_benchmark_B();
	run_benchmark_C();
	run_benchmark_D();
}

void test_bonus_problem()
{
	kprintf("Running the bonus problem ...\n");
	resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	 
	resume(create(joker, 1024, 20, "joker", 0));

	resume(create(iobnd, 1024, 30, "io bound 1", 0));
	resume(create(iobnd, 1024, 30, "io bound 2", 0));
}

static void run_benchmark_A()
{
	kprintf("Running the benchmark A ...\n");
	resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 3", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 4", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 5", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 6", 0));
}
static void run_benchmark_B()
{
	kprintf("Running the benchmark B ...\n");
	resume(create(iobnd, 1024, 30, "io bound 1", 0));
	resume(create(iobnd, 1024, 30, "io bound 2", 0));
	resume(create(iobnd, 1024, 30, "io bound 3", 0));
	resume(create(iobnd, 1024, 30, "io bound 4", 0));
	resume(create(iobnd, 1024, 30, "io bound 5", 0));
	resume(create(iobnd, 1024, 30, "io bound 6", 0));
}
static void run_benchmark_C()
{
	kprintf("Running the benchmark C ...\n");
	resume(create(iobnd, 1024, 30, "io bound 1", 0));
	resume(create(iobnd, 1024, 30, "io bound 2", 0));
	resume(create(iobnd, 1024, 30, "io bound 3", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 3", 0));
}
static void run_benchmark_D()
{
	kprintf("Running the benchmark D ...\n");
	resume(create(cpubnd, 1024, 30, "cpu bound 1", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 2", 0));
	sleepms(3000);
	chprio(getpid(),10);
	resume(create(cpubnd, 1024, 30, "cpu bound 3", 0));
	resume(create(cpubnd, 1024, 30, "cpu bound 4", 0));
}