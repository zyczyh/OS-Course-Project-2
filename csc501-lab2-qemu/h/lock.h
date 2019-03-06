
#ifndef _LOCK_H_
#define _LOCK_H_

#ifndef	NLOCK
#define	NLOCK		50	/* number of locks, if not defined	*/
#endif

#define	LFREE	'\01'		/* this lock is free		*/
#define	LUSED	'\02'		/* this lock is used		*/
#define READ	0			/* lock type for reader */
#define WRITE	1			/* lock type for writer */
#define BLANK	2			/* lock type for newly created lock */

struct	lentry	{		/* semaphore table entry		*/
	char	lstate;		/* the state LFREE or LUSED		*/
	int ltype;		/* type of the lock(READ, WRITE or BLANK) */
	int	lqhead;		/* q index of head of list		*/
	int	lqtail;		/* q index of tail of list		*/
	int readernum;	/* number of readers in critical section */
};
extern	struct	lentry	lock[];
extern	int	nextlock;

#define	isbadlock(l)	(l<0 || l>=NSEM)

#endif