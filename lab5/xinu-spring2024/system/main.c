/*  main.c  - main */

#include <xinu.h>

#define CONTENT "Hello World"
#define LEN (LIF_AREA_DIRECT+LIF_AREA_INDIR)

char buffer[LEN];
char buffer2[LEN];
did32 fd;
int status_val;
int i;
int iter;

void test_seek(int start_pos){

	// // test seek
	status_val = seek(fd, start_pos);
	kprintf("status_val for seek: %d\n", status_val);

	// test getc after seeking
	iter=10;
	for(i=0;i<iter;i++){
		status_val = getc(fd);
		if(status_val!=buffer[i+start_pos]){
			kprintf("[error] i=%d, buffer[i+start_pos] = %d, status_val for seek: %d\n", i,buffer[i+start_pos],status_val);
			// exit();
		}
	}
}

process main() {
	// print some constants
	kprintf("LIF_AREA_DIRECT: %d\n", LIF_AREA_DIRECT);
	kprintf("LIF_AREA_INDIR: %d\n", LIF_AREA_INDIR);
	kprintf("LIF_AREA_2INDIR: %d\n", LIF_AREA_2INDIR);
	kprintf("LIF_AREA_3INDIR: %d\n", LIF_AREA_3INDIR);

	lifscreate(RAM0, 128, 102400); // initialize the ramdisk

	

	// open the file for write

	fd = open(LIFILESYS, "index.txt", "rw");
	kprintf("fd: %d\n", fd);

	// fill buffer with random values
	for(i=0;i<LEN;i++){
		buffer[i] = (clktime%256);
	}

	// test write
	status_val = write(fd, buffer, LEN);
	kprintf("status_val for write: %d\n", status_val);

	test_seek(100);
	test_seek(LIF_AREA_DIRECT-3);
	test_seek(LIF_AREA_DIRECT+500);

	

	status_val = close(fd);
	kprintf("status_val for close: %d\n", status_val);

	// re-open the file for read
	fd = open(LIFILESYS, "index.txt", "r");
	kprintf("fd: %d\n", fd);
	
	// test getc
	for(i=0;i<LEN;i++){
		status_val = getc(fd);
		if(status_val!=buffer[i]){
			kprintf("[error] i=%d, buffer[i] = %d, status_val for getc: %d\n", i,buffer[i],status_val);
			// exit();
		}
	}

	status_val = seek(fd, 0);
	kprintf("status_val for seek: %d\n", status_val);

	// test read
	status_val = read(fd, buffer2, LEN);

	for (i = 0; i < LEN; ++i) {
		if(buffer2[i]!=buffer[i]){
			kprintf("[error] i=%d, buffer[i] = %d, buffer2[i] = %d for read: %d\n", i,buffer[i],buffer2[i]);
			// exit();
		}
	}


	status_val = close(fd);
	kprintf("status_val for close: %d\n", status_val);

	return 0;
}
