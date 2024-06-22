/*  main.c  - main */

#include <xinu.h>

#define CONTENT "Hello World"

#define LEN (LIF_AREA_DIRECT+LIF_AREA_INDIR)

process main() {
	// print some constants
	kprintf("LIF_AREA_DIRECT: %d\n", LIF_AREA_DIRECT);
	kprintf("LIF_AREA_INDIR: %d\n", LIF_AREA_INDIR);
	kprintf("LIF_AREA_2INDIR: %d\n", LIF_AREA_2INDIR);
	kprintf("LIF_AREA_3INDIR: %d\n", LIF_AREA_3INDIR);

	lifscreate(RAM0, 128, 102400); // initialize the ramdisk
	
	did32 fd;
	int status;
	int i;
	char writeBuffer[LEN];
	char data;

	memset(writeBuffer, NULLCH, LEN + 1);

	//1. open the file for write
	fd = open(LIFILESYS, "index.txt", "rw");
	kprintf("open fd: %d\n", fd);

	//2. test write with size of file less than LIF_AREA_DIRECT
	status = write(fd, CONTENT, 11);
	kprintf("status for write direct block: %d\n", status);

	//read content of file
	kprintf("Content of the file:");
	for (i = 0; i < 11; ++i) {
		seek(fd, i);
		kprintf("%c",getc(fd));
	}
	kprintf("\n");

	//3. Test write data with size of file large than  LIF_AREA_DIRECT
	for(i=0; i < LEN; i++){
		writeBuffer[i] = clktime % 256;
	}
	status = write(fd, LIF_AREA_DIRECT, LEN);
	kprintf("status for write indirect block: %d\n", status);

	//4. Test seek data;
	status = seek(fd, LIF_AREA_DIRECT-3);
	kprintf("status for seek: %d\n", status);
	//5. test putc
	status = putc(fd, 'X');
	kprintf("status for putc: %d\n", status);

	//6. Test close
	status = close(fd);
	kprintf("status for close: %d\n", status);

	// re-open the file for read
	fd = open(LIFILESYS, "index.txt", "r");
	kprintf("fd: %d\n", fd);

	//7. Test read
	char readBuffer[LEN + 1];
	memset(readBuffer, NULLCH, LEN + 1);
	status = read(fd, readBuffer, LEN);
	kprintf("Content of the file:");
	for (i = 0; i < LEN; ++i) {
		kprintf("%c", readBuffer[i]);
	}
	kprintf("\n");

	return 0;
}
