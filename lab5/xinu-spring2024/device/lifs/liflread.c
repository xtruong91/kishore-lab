/* liflread.c - liflread */

#include <xinu.h>

/*------------------------------------------------------------------------
 * liflread  -  Read from a previously opened local file
 *------------------------------------------------------------------------
 */
devcall	liflread (
	  struct dentry *devptr,	/* Entry in device switch table */
	  char	*buff,			/* Buffer to hold bytes		*/
	  int32	count			/* Max bytes to read		*/
	)
{
	uint32	numread;		/* Number of bytes read		*/
	int32	nxtbyte;		/* Character or SYSERR/EOF	*/

	if (count < 0) {
		return SYSERR;
	}

	/* Iterate and use liflgetc to read individual bytes */

	for (numread=0 ; numread < count ; numread++) {
		nxtbyte = liflgetc(devptr);
		if (nxtbyte == SYSERR) {
			return SYSERR;
		} else if (nxtbyte == EOF) {	/* EOF before finished */
		    if (numread == 0) {
			return EOF;
		    } else {
			return numread;
		    }
		} else {
			*buff++ = (char) (0xff & nxtbyte);
		}
	}
	return numread;
}
