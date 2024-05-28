
#include <xinu.h>

void returnwrapper(void){
    kprintf("Process terminating\n");
	kill(getpid());
}