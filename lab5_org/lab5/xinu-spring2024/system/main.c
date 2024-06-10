/*  main.c  - main */

#include <xinu.h>

#define CONTENT "Hello World"
#define LEN 11

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

	// open the file for write

	fd = open(LIFILESYS, "index.txt", "rw");
	kprintf("fd: %d\n", fd);

	// test write
	status = write(fd, CONTENT, LEN);
	kprintf("status for write: %d\n", status);

	// test seek
	status = seek(fd, 5);
	kprintf("status for seek: %d\n", status);
    //write overflow data for test indirect blocks;
	for(i=11; i < 10000; i++)
	{
		seek(fd, i);
		putc(fd, 'X');
	}
	// test putc
	status = putc(fd, 'X');
	kprintf("status for putc: %d\n", status);

	status = close(fd);
	kprintf("status for close: %d\n", status);

	// re-open the file for read
	fd = open(LIFILESYS, "index.txt", "r");
	kprintf("fd: %d\n", fd);

	char buf[LEN + 1];
	memset(buf, NULLCH, LEN + 1);

	// test read
	status = read(fd, buf, LEN);
	buf[LEN] = '\0';
	kprintf("Content of the file:");
	for (i = 0; i < LEN; ++i) {
		kprintf("%c", buf[i]);
	}
	kprintf("\n");
}
