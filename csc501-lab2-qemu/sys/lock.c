#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

int lock(int ldes1, int type, int priority)
{
	STATWORD ps;    
	struct	lentry	*lptr;
	struct	pentry	*pptr;

	disable(ps);
	if(isbadlock(ldes1) || (lptr= &lock[ldes1])->lstate==LFREE) 
	{
		restore(ps);
		return(SYSERR);
	}
	pptr = &proctab[currpid];
	if(pptr->plstatus == LOCKDNE && ldes1 == pptr->plock)
	{
		restore(ps);
		return(SYSERR);
	}
	if(lptr->ltype == BLANK) 
	{
		lptr->ltype = type;
		restore(ps);
		return(OK);
	}
	else if(lptr->ltype == READ && type == READ && priority > lastkey(lptr->lqtail))
	{
		lptr->ltype = type;
		restore(ps);
		return(OK);
	}
	else
	{
		pptr->pstate = PRWAIT;
		pptr->plock = ldes1;
		pptr->plstatus = LOCKE;
		pptr->pltype = type;
		insert(currpid, lptr->lqhead, priority);
		pptr->pwaitret = OK;
		resched();
		restore(ps);
		return pptr->pwaitret;
	}
}

