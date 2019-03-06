#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

int releaseall(int numlocks, long args, ...)
{
	STATWORD ps;
    disable(ps);

    int i;
    int currldes;
    int iswrong = 0;
    for(i = 0; i < numlocks; i++) {
        currldes = (int)*((&args) + i);
        if(release(currldes) == SYSERR) {
            iswrong++;
        }
    }
    
    resched();
    if(iswrong > 0) {
        restore(ps);
        return SYSERR;
    }
    else {
        restore(ps);
        return OK;
    }
}

void release(int ldes)
{
	struct	pentry	*pptr;
	struct	lentry	*lptr;
	int next = 0;

	if(isbadlock(ldes))
	{
		return SYSERR;
	}
	pptr = &proctab[currpid];
	lptr = &lock[ldes];
	if(pptr->pltype == READ)
	{
		lptr->readernum = lptr->readernum - 1;
		if(lptr->readernum == 0)
		{
			lptr->ltype = BLANK;
			if(nonempty(lptr->lqhead) == TRUE)
			{
				next = getlast(lptr->lqtail);
				lptr->ltype = WRITE;
				ready(next, RESCHNO);
			}
		}
	}
	else if(pptr->pltype == WRITE)
	{
		lptr->ltype = BLANK;
		if(nonempty(lptr->lqhead) == TRUE)
		{
			next = getlast(lptr->lqtail);
			if(proctab[next].pltype == WRITE)
			{
				lptr->ltype = WRITE;
				ready(next, RESCHNO);
			}
			else
			{
				lptr->ltype = READ;
				ready(next, RESCHNO);
				while(nonempty(lptr->lqhead) == TRUE && proctab[q[lptr->lqtail].qprev].pltype != WRITE)
				{
					next = getlast(lptr->lqtail);
					ready(next, RESCHNO);
				}
			}
		}
	}
	else
	{
		return SYSERR;
	}
	return OK;
}
