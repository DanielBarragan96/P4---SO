#include "scheduler.h"

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int totthreads;
extern int tcount;
extern int newthread;
extern int blockevent;
extern int unblockevent;

#define Q 2

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];

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
	}

	if(event==BLOCKTHREAD)
	{

		threads[currthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],currthread);

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

	int rounds = 0;
	// Ejecuta todos los hilos actuales que estén en ready
	for(;;)
	{			
		old=currthread;	
		// Sacar un hilo de la cola de listos
		next = _dequeue(&ready);		
		// Si ya no hay procesos listos, salir
		if((MAXTHREAD>rounds)&&(0>next || totthreads<next))	
			return;
		threads[next].status=RUNNING;
		for(int quant=0;Q>quant;quant++)
		{// Cambia contexto de los hilos
			_swapthreads(old,next);		
			WaitThread(threads[next].tid);
		}
		rounds++;
	}
}

