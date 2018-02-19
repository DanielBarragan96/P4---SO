#include "scheduler.h"

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

void scheduler(int event)
{
	int old,next;
	int changethread=0;
	int waitingthread;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		threads[newthread].status=READY;
		priorities[newthread] = 0;
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
	}

	// Ejecuta todos los hilos actuales que estén en ready
	for(;;)
	{	
		int priority=0;	
		for(int count=0;MAXTHREAD>count;count++)
		{
			if(priorities[count] > priority)
			{// Obtener la mayor prioridad
				priority = priorities[count];
			}
		}
		for(int p = 0;priority>=p;p++)
		{
			for(int count=0;MAXTHREAD>count;count++)
			{
				if(priorities[count] == p)
				{
					old=currthread;	
					// Sacar un hilo de la cola de listos
					next = _dequeue(&ready);		
					// Si ya no hay procesos listos, salir
					if(0>next || totthreads<next)	
						return;
					threads[next].status=RUNNING;
					priorities[next] += 1;
					// Cambia contexto de los hilos
					_swapthreads(old,next);		
					WaitThread(threads[next].tid);
				}
			}
		}
	}
}

