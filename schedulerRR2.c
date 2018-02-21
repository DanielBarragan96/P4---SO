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

int second_q = 0;

void scheduler(int event)
{
	int old,next;
	int changethread=0;
	int waitingthread;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		threads[newthread].status=READY;
		_enqueue(&ready,newthread);
		second_q = 2;
	}

	if(event==BLOCKTHREAD)
	{

		threads[currthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],currthread);

		changethread=1;
		second_q = 2;
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
		second_q = 2;
	}
	if(event==TIMER)
	{
		second_q++;
		if(2 <= second_q)
		{
			second_q = 2;
			threads[currthread].status=READY;
			_enqueue(&ready,currthread);
			changethread=1;
		}
	}
	
	if(changethread && second_q==2)
	{
		second_q = 0;
		old=currthread;	
		// Sacar un hilo de la cola de listos
		next = _dequeue(&ready);
		threads[next].status=RUNNING;
		_swapthreads(old,next);	
	}
}

