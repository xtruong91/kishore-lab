#include <xinu.h>

/*------------------------------------------------------------------------
 *  multilevelfbq  -  Use of priority list in place of multilevel feedback queue
 *------------------------------------------------------------------------
 */

struct mlqentry mlqtable[60][NPROC];

void mlqenqueue(pid32 pid) {
	//kprintf("ENQUEUE %d\n", pid);
	struct procent *proc = &proctab[pid];
	int priority = proc->prprio;
	mlqtable[priority][mlqstartendtable[priority].end%NPROC].processID = pid;
	mlqstartendtable[priority].end++;
}

pid32 mlqdequeue() {
	int i = 59;
	while (mlqtable[i][mlqstartendtable[i].start%NPROC].processID == EMPTY) {
		i--;
		if (i == -1)
			return 0;
	}

	pid32 ret = mlqtable[i][mlqstartendtable[i].start%NPROC].processID;
	mlqtable[i][mlqstartendtable[i].start%NPROC].processID = EMPTY;
	mlqstartendtable[i].start++;

	if (ret == -1)
		return 0;

	//kprintf("DEQUEUE %d %d\n", EMPTY, ret);
	return ret;
}

pid32 mlqprioritypeek() {
	//kprintf("MLQPEEK\n");
	int i = 59;
	while (mlqtable[i][0].processID == -1){
		i--;
	}
	return i;

}
