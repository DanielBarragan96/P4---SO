all: example exampleRR exampleRR2 exampleRetro schedulerFCFS.o libthreads.o 

example: example.c libthreads.o schedulerFCFS.o
	gcc -o example example.c libthreads.o schedulerFCFS.o
	
libthreads: libthreads.c libthreads.h commondata.h
	gcc -c libthreads.c
	
schedulerFCFS: schedulerFCFS.c libthreads.h commondata.h
	gcc -c schedulerFCFS.c

exampleRR: example.c libthreads.o schedulerRR.o
	gcc -o exampleRR example.c libthreads.o schedulerRR.o
	
exampleRR2: example.c libthreads.o schedulerRR2.o
	gcc -o exampleRR2 example.c libthreads.o schedulerRR2.o
	
exampleRetro: example.c libthreads.o schedulerRetro.o
	gcc -o exampleRetro example.c libthreads.o schedulerRetro.o
