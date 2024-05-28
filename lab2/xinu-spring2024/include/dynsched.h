#include<kernel.h>

struct dynsched_tab
{ 
	uint16 ts_tqexp; // new priority: CPU-bound (time quantum expired)
	uint16 ts_slpret; // new priority: I/O-bound (called sleepms())
	uint16 ts_quantum; // time slice associated with priority level
};

extern	struct	dynsched_tab dynprio[];
