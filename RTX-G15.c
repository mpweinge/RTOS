/*
 *  Main.c
 *  
 *
 *  Created by Michael Weingert on 11-11-05.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

//INCLUDE LIBs
#include <stdlib.h>
#include <unistd.h>			// getpid() definition
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>

//INCLUDE HEADERS
#include "Constants.h"
#include "Structures.h"
#include "KernelPointers.h"
#include "Clean.h"
#include "RPQ.h"
#include "RTX-G15.h"
#include "Initialize.h"
#include "TraceBuffer.h"

caddr_t mmap_ptr;

char * kbFileName = "Keyboard_Shared_Memory";
char * CRTFileName = "CRT_Shared_Memory";

int bufsize = BUFFERSIZE;
int i;
int* tempPtr;
int status;
initTable* tempTablePtr;

inputbuf * test;

void InterruptHandler(int SigNum)
{
	//forward it onto interrupt handler
	interruptHand(SigNum);
}

//int InitializeKernel()
int main()
{	
	int status;
	MsgEnv* tempMessage = NULL;
	PCB* temp = NULL;
	// catch signals so we can clean up everything before exitting
	// signals defined in /usr/include/signal.h
	// e.g. when we recieved an interrupt signal SIGINT, call interrupthandler()
	//sigset(SIGINT,cleanup);		// catch kill signals 
	sigset(SIGINT, InterruptHandler);
	//sigset(SIGBUS,InterruptHandler);		// catch bus errors
	//sigset(SIGHUP,InterruptHandler);		
	//sigset(SIGILL,InterruptHandler);		// illegal instruction
	//sigset(SIGQUIT,InterruptHandler);
	//sigset(SIGABRT,InterruptHandler);
	//sigset(SIGTERM,InterruptHandler);
	//sigset(SIGSEGV,InterruptHandler);	// catch segmentation faults
	sigset(SIGALRM, InterruptHandler);
	
	// signal from keyboard reader is SIGUSR1 (user-defined signal)
	// When there is input from the keyboard, call the kbd_handler() routine
	//sigset(SIGUSR1, keyboardHndlr);
	sigset(SIGUSR1, InterruptHandler);
	sigset(SIGUSR2, InterruptHandler);
	
	//------INTIALIZE RTX COMPONENTS------//
	
	
	printf("\nInitializing Init Table");
	fflush(stdout);
	//Create Init Table
	if(CreateInitializationTable() == ERROR)
	{
		main_cleanup();
		printf("Error creating Initialization Table");
		exit(0);
	}
	
	printf("\nInitializing MSG Queues");
	fflush(stdout);
	
	//Create MSG Queues
	if(Initialize_MSG_Queues() == ERROR)
	{
		main_cleanup();
		printf("Error creating Free Message Queue");
		exit(0);
	}
	
	printf("\nInitializing PCB Queues");
	fflush(stdout);
	
	//Create PCB Queues
	if(Initialize_PCB_Queues() == ERROR)
	{
		main_cleanup();
		printf("Error creating PCB Queues");
		exit(0);
	}
	
	printf("\nInitializing PCBs");
	fflush(stdout);
	
	//Set PCBs and Context PCB Queues
	if(Initialize_PCBs() == ERROR)
	{
		main_cleanup();
		printf("Error Initializing PCBs");
		exit(0);
	}

	InitializeTraceBufferSEND();
	InitializeTraceBufferRECEIVE();
	
	//------INITIALIZE RTX COMPLETE-----//
	
	printf("\nInitialize RTX Complete");
	fflush(stdout);
	
	
	kbfid = open(kbFileName, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755);
	if (kbfid < 0)
	{
		printf("Error creating the memory mapped %s file", kbFileName);
		exit(0);
	}
	
	status = ftruncate(kbfid, bufsize);
	if (status)
	{
		printf("Failed to ftruncate the file %s, status = %d", kbFileName, status);
		exit(0);
	}
	
	mmap_ptr = mmap((caddr_t) 0,   /* Memory location, 0 lets O/S choose */
					bufsize,              /* How many bytes to mmap */
					PROT_READ | PROT_WRITE, /* Read and write permissions */
					MAP_SHARED,    /* Accessible by another process */
					kbfid,           /* the file associated with mmap */
					(off_t) 0);    /* Offset within a page frame */
    	if (mmap_ptr == MAP_FAILED){
		printf("Parent's memory map has failed, about to quit!\n");
		exit(0);  // do cleanup and terminate
   	 };
	
	in_mem_p = (inputbuf *) mmap_ptr;   // pointer to shared memory
	test = in_mem_p;
	// we can now use 'in_mem_p' as a standard C pointer to access 
	// the created shared memory segment 	
	
	// now start doing whatever work you are supposed to do
	// in this case, do nothing; only the keyboard handler will do work
	in_mem_p->ok_flag = 0;
	printf("\nType one of the RTOS commands and enjoy :D!\n\n");
	
	char childarg1[20], childarg2[20]; 
	
	int mypid = getpid();
	sprintf(childarg1, "%d", mypid); // convert to string to pass to child
    	sprintf(childarg2, "%d", kbfid);   // convert the file identifier
	
	crtfid = open(CRTFileName, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755);
	if (crtfid < 0)
	{
		printf("Error creating the memory mapped %s file", CRTFileName);
		exit(0);
	}
	
	
	status = ftruncate(crtfid, bufsize);
	if (status)
	{
		printf("Failed to ftruncate the file %s, status = %d", CRTFileName, status);
		exit(0);
	}
	
	mmap_ptr = mmap((caddr_t) 0,   /* Memory location, 0 lets O/S choose */
					bufsize,              /* How many bytes to mmap */
					PROT_READ | PROT_WRITE, /* Read and write permissions */
					MAP_SHARED,    /* Accessible by another process */
					crtfid,           /* the file associated with mmap */
					(off_t) 0);    /* Offset within a page frame */
    if (mmap_ptr == MAP_FAILED){
		printf("Parent's memory map has failed, about to quit!\n");
		exit(0);  // do cleanup and terminate
    };
	
	out_mem_p = (inputbuf *) mmap_ptr;   // pointer to shared memory
	// we can now use 'in_mem_p' as a standard C pointer to access 
	// the created shared memory segment 	
	
	// now start doing whatever work you are supposed to do
	// in this case, do nothing; only the keyboard handler will do work
	out_mem_p->ok_flag = 0;

	kb_pid = fork();
	if (kb_pid == 0)	// is this the child process ?
	{
		execl("./Keyboard", "Keyboard", childarg1, childarg2, (char *)0);
		// should never reach here
		fprintf(stderr,"demo: can't exec keyboard, errno %d\n",errno);
		//cleanup();
		exit(1);
	}
	
	sprintf(childarg1, "%d", mypid); // convert to string to pass to child
   	sprintf(childarg2, "%d", crtfid);   // convert the file identifier
	
	crt_pid = fork();
	if (crt_pid == 0)	// is this the child process ?
	{
		execl("./CRT", "CRT", childarg1, childarg2, (char *)0);
		// should never reach here
		fprintf(stderr,"demo: can't exec CRT, errno %d\n",errno);
		//cleanup();
		exit(1);
	};
	
	// allocate a shared memory region using mmap 
	// the child process also uses this region
	
	InitializeGlobals();
	
	ualarm(100000,100000); /// perhaps create this earlier
	
	printf("\nDone Initialization");
	fflush(stdout);
	
	curses_init();	

	//Hard code the first dequeue
	curr_proc1 = RPQ_dequeue(RPQ);
	curr_proc1->state = EXECUTING;
	curr_proc1->main();
	
	//
	
	while(1) { }
	
	//while (1)
	//{
	//	usleep(100000);
	//}
	
	// should never reach here, but in case we do, clean up after ourselves
	main_cleanup();
	exit(1);
} // main
