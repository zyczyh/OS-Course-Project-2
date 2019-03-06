#include <conf.h>
#include <lock.h>
#include <q.h>
#include <kernel.h>

struct	lentry	lock[NLOCK];
int	nextlock;

/* initialize locks */ 

void linit()
{
	struct	lentry	*lptr;
	int i = 0;

	nextlock = NLOCK - 1;
	for (i = 0 ; i < NLOCK ; i = i + 1) 
	{
		(lptr = &lock[i])->lstate = LFREE;
		lptr->readernum = 0;
		lptr->lqtail = 1 + (lptr->lqhead = newqueue());
	}
}