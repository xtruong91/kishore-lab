/* lifsetup.c - lifsetup */

#include <xinu.h>

/*------------------------------------------------------------------------
 * lifsetup  -  Set a file's index block and data block for the current
 *		 file position (assumes file mutex held)
 *------------------------------------------------------------------------
 */
status	lifsetup (
	  struct liflcblk  *lifptr	/* Pointer to slave file device	*/
	)
{
	dbid32	dnum;			/* Data block to fetch		*/
	ibid32	ibnum;			/* I-block number during search	*/
	struct	ldentry	*ldptr;		/* Ptr to file entry in dir.	*/
	struct	lifiblk	*ibptr;		/* Ptr to in-memory index block	*/
	int32	dindex;			/* Index into array in an index	*/
					/*   block			*/
	dbid32	*dnumptr;		/* where to store the newly allocated block number */


	/* Obtain exclusive access to the directory */

	wait(Lif_data.lif_mutex);

	/* Get pointers to in-memory directory, file's entry in the	*/
	/*	directory, and the in-memory index block		*/

	ldptr = lifptr->lifdirptr;
	ibptr = &lifptr->lifiblock;

	/* If existing index block or data block changed, write to disk	*/

	if (lifptr->lifibdirty || lifptr->lifdbdirty) {
		lifflush(lifptr);
	}
	ibnum = lifptr->lifinum;		/* Get ID of curr. index block	*/

	/* If there is no index block in memory (e.g., because the file	*/
	/*	was just opened), either load the index block of	*/
	/*	the file or allocate a new index block		*/

	if (ibnum == LF_INULL) {

		/* Check directory entry to see if index block exists	*/

		ibnum = ldptr->ld_ilist;
		if (ibnum == LF_INULL) { /* Empty file - get new i-block*/
			ibnum = lifiballoc();
			lifibclear(ibptr, 0);
			ldptr->ld_ilist = ibnum;
			// dirty because the i-block number is updated
			lifptr->lifibdirty = TRUE;
		} else {		/* Nonempty - read first and only i-block*/
	 		lifibget(Lif_data.lif_dskdev, ibnum, ibptr);
		}
		lifptr->lifinum = ibnum;
	}

	/* At this point, an index block is in memory (pointed to by ibptr), 
	 * but the file's position might point to data reachable 
	 * only through indirect blocks */

	if (lifptr->lifpos >= LIF_AREA_DIRECT) {
		/* TODO: implement indirect blocks support */
		kprintf("indirect blocks not implemented\n");
		exit();
	} else {
		dindex = (lifptr->lifpos & LF_IMASK) >> 9;
		dnumptr = &lifptr->lifiblock.ib_dba[dindex];
		dnum = *dnumptr;
	}

	/* dnum is set to the correct data block that covers the current file position */

	/* At this point, dnum is either LF_DNULL (meaning the position is beyond the file content)
	 *  or covers the current file position (i.e., position lifptr->lifpos).  
	 *  The	next step consists of loading the correct data block.	*/

	if (dnum == LF_DNULL) {		/* Allocate new data block */
		dnum = lifdballoc((struct lifdbfree *)&lifptr->lifdblock);
		*dnumptr = dnum;
		// dirty because the content in the i-block (its data blocks) is updated
		lifptr->lifibdirty = TRUE;

	/* If data block index does not match current data block, read	*/
	/*   the correct data block from disk				*/

	/* dnum is the correct data block number */
	/* lifptr->lifdnum is the "cached" data block number */ 

	} else if ( dnum != lifptr->lifdnum) {
		read(Lif_data.lif_dskdev, (char *)lifptr->lifdblock, dnum);
		lifptr->lifdbdirty = FALSE;
	}
	lifptr->lifdnum = dnum;

	/* Use current file offset to set the pointer to the next byte	*/
	/*   within the data block					*/

	lifptr->lifbyte = &lifptr->lifdblock[lifptr->lifpos & LF_DMASK];
	signal(Lif_data.lif_mutex);
	return OK;
}
