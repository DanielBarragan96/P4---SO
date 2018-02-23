#define NEW 0
#define END 1
#define READY 2
#define RUNNING 3
#define BLOCKED 4


#define TIMER 0
#define ENDTHREAD 1
#define NEWTHREAD 2
#define BLOCKTHREAD 3
#define UNBLOCKTHREAD 4

#define MAXTHREAD 10

typedef struct _QUEUE {
	int elements[MAXTHREAD];
	int head;
	int tail;
} QUEUE;

typedef struct THREAD {
	int tid;			// Identificador del hilo
	int status;			// Status END(0),RUNNING(1),BLOCKING(2)
	int retstat;		// Valor de retorno
	ucontext_t thread_context;	// Contexto 
	char iterator_stack[SIGSTKSZ];
	struct THREAD *next;
} THANDLER; 