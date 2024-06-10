/* lifflush.c - lifflush */

#include <xinu.h>

/*------------------------------------------------------------------------
 * lifflush  -  Flush directory, data block, and index block for an open
 *			file (assumes file mutex is held)
 *------------------------------------------------------------------------
 */
status	lifflush (
	  struct liflcblk  *lifptr	/* Ptr to file pseudo device	*/
	)
{

	if (lifptr->lifstate == LF_FREE) {
		return SYSERR;
	}

	/* Write the directory if it has changed */

	if (Lif_data.lif_dirdirty) {
		write(Lif_data.lif_dskdev, (char *)&Lif_data.lif_dir,
							LF_AREA_DIR);
		Lif_data.lif_dirdirty = FALSE;
	}

	/* Write data block if it has changed */

	if (lifptr->lifdbdirty) {
		write(Lif_data.lif_dskdev, lifptr->lifdblock, lifptr->lifdnum);
		lifptr->lifdbdirty = FALSE;
	}

	/* Write i-block if it has changed */

	if (lifptr->lifibdirty) {
		lifibput(Lif_data.lif_dskdev, lifptr->lifinum, &lifptr->lifiblock);
		lifptr->lifibdirty = FALSE;
	}
	
	return OK;
}
