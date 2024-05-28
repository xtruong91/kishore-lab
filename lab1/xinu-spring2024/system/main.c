// system/main.c

#include <xinu.h>

void procPrintInfo(void);
void procPrintParentState1(void);
void procPrintParentState2(void);
void procPrintParentState3(void);

#define SLEEP_MS 2500

process	main(void)
{
	kprintf("Test process executing main(): PID = %d\n", getpid());
	procPrintInfo();
	sleepms(SLEEP_MS);

	// check create() function
	resume(create(procPrintInfo, 1024, 20, "create1", 0));
	sleepms(SLEEP_MS);

	// check createvariant()
	resume(createvariant(procPrintInfo, 1024, 0, "createvariantA", 0));
	sleepms(SLEEP_MS);
	resume(createvariant(procPrintInfo, 8193, 45, "createvariantB", 0));
	sleepms(SLEEP_MS);

	// check createvariant2()
	resume(createvariant2(procPrintInfo, 1024, 20, "createvariant2A", 0));
	sleepms(SLEEP_MS);
	resume(createvariant2(procPrintInfo, 8193, 45, "createvariant2B", 0));
	sleepms(SLEEP_MS);

	// check getparentprstate()
	resume(create(procPrintParentState1, 1024, 20, "prparentstate1", 0)); // this should print 4 [PR_SLEEP]
	sleepms(SLEEP_MS);

	procPrintParentState3(); // this should print -1 [SYSERR]
	sleepms(SLEEP_MS);

	resume(create(procPrintParentState2, 1024, 20, "prparentstate2", 0)); // this should print 0 [PR_FREE]

	return OK;

}

void procPrintParentState1(void){
	pid32 pid = getpid();
	kprintf("\n********************\n");
	kprintf("Process PID: %d\n", pid);
	kprintf("Parent process state: %d\n", getparentprstate(pid));
	kprintf("********************\n");
}

void procPrintParentState2(void){
	sleepms(SLEEP_MS);
	pid32 pid = getpid();
	kprintf("\n********************\n");
	kprintf("Process PID: %d\n", pid);
	kprintf("Parent process state: %d\n", getparentprstate(pid));
	kprintf("********************\n");
}

void procPrintParentState3(void){

	pid32 pid = -1;
	kprintf("\n********************\n");
	kprintf("Process PID: %d\n", pid);
	kprintf("Parent process state: %d\n", getparentprstate(pid));
	kprintf("********************\n");
}


/*------------------------------------------------------------------------ *
procPrintInfo
*------------------------------------------------------------------------ */

void procPrintInfo(void) {
	pid32 pid = getpid();
	kprintf("\n********************\n");
	kprintf("Process PID: %d\n", pid);
	kprintf("Process name: %s\n", proctab[pid].prname );
	kprintf("Process stack size: %d\n", proctab[pid].prstklen);
	kprintf("Process priority: %d\n", proctab[pid].prprio);
	kprintf("********************\n");
	// while (1) {
	// 	 putc(CONSOLE, 'B');
	// }
}
