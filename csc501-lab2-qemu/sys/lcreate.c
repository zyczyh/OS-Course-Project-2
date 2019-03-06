#include <lock.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

LOCAL int newloc();

int lcreate()
{
	STATWORD ps;    
	int	loc;

	disable(ps);
	if ((loc = newloc()) == SYSERR) 
	{
		restore(ps);
		return(SYSERR);
	}
	lock[loc].ltype = BLANK;
	lock[loc].readernum = 0;
	/* lqhead and lqtail were initialized at system startup */
	restore(ps);
	return(loc);
}

LOCAL int newloc()
{
	int	loc;
	int	i;

	for (i=0 ; i<NLOCK ; i++) {
		loc=nextlock--;
		if (nextlock < 0)
			nextlock = NLOCK-1;
		if (lock[loc].lstate == LFREE) {
			lock[loc].lstate = LUSED;
			return(loc);
		}
	}
	return(SYSERR);
}