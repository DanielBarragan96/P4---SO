#include "scheduler.h"
#include "stdio.h"

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int totthreads;
extern int tcount;
extern int newthread;
extern int blockevent;
extern int unblockevent;

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];
volatile static int priorities[MAXTHREAD];

int feedbackPriorityChangeT()
{
	for(int i=0 ; MAXTHREAD>i ; i++)
	{
		if(priorities[i] < priorities[currthread])
		{
			return 0;
		}
	}
	return 1;
}

void scheduler(int event)
{
	int old,next;
	int changethread=0;
	int waitingthread;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		threads[newthread].status=READY;
		priorities[currthread] = 0;
		_enqueue(&ready,newthread);
	}

	if(event==BLOCKTHREAD)
	{

		threads[currthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],currthread);
		priorities[currthread] += 1;

		changethread=1;
	}

	if(event==ENDTHREAD)
	{
		threads[currthread].status=END;

		// Si el hilo que termina tiene bloqueado un hilo
		// ponlo en la cola de listos para desbloquearlo
		if(!_emptyq(&waitinginevent[currthread]))
		{
			waitingthread=_dequeue(&waitinginevent[currthread]);
			threads[waitingthread].status=READY;
			_enqueue(&ready,waitingthread);
		}
		changethread=1;
		priorities[currthread] = 0;
	}

	if(event==TIMER)
	{
		if(feedbackPriorityChangeT())
		{
			changethread=1;
			priorities[currthread] -= 1;
		}
		threads[currthread].status=READY;
		_enqueue(&ready,currthread);
	}
	
	if(changethread)
	{
		priorities[currthread] += 1;
		old=currthread;	
		// Sacar un hilo de la cola de listos
		next = _dequeue(&ready);
		threads[next].status=RUNNING;
		_swapthreads(old,next);	
	}
}
