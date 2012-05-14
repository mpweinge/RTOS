/*
 *  Interrupt.c
 *  
 *
 *  Created by Michael Weingert on 11-11-06.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Interrupt.h"
#include "Primitives.h"
#include "KernelPointers.h"
#include "RPQ.h"
#include "Structures.h"

#include <stdio.h>
#include <stdlib.h>

int status;
int buffersize = BUFFERSIZE;

inputbuf * in_mem_p1;
inputbuf * out_mem_p1;
PCB * KB;
PCB * CRT;
PCB * CLOCK;

int mscount;

char * KBFileName = "Keyboard_Shared_Memory";
char * crtFileName = "CRT_Shared_Memory";

void InitializeGlobals(void)
{
	in_mem_p1 = in_mem_p;
	out_mem_p1 = out_mem_p;
	KB = PCBAddress[KB_IPROCESS_PID-1];
	CRT = PCBAddress[CRT_IPROCESS_PID-1];
	CLOCK = PCBAddress[CLOCK_IPROCESS_PID-1];
	mscount = 0;
	InitializeDelayQueue();
}

void interruptHand(int signal)
{
	atomic(ON);
	switch (signal)
	{
		case SIGUSR1:
			KeyboardHandler();
			break;
		case SIGUSR2:
			CRTHandler();
			break;
		case SIGSEGV:
			SegFault();
			break;
		case SIGALRM:
			ClockHandler();
			break;
		default:
			Terminate();
			break;
	}
	atomic(OFF);
}

void InitializeDelayQueue()
{
	int i;
	
	DelayQueueHead = (DelayItem*) malloc(sizeof(DelayItem));
	
	DelayItem* temp = DelayQueueHead;
	
	for(i=0; i<NUM_PROCESSES; i++)
	{
		temp->next = (DelayItem*) malloc(sizeof(DelayItem));
		temp->time = -1;
		temp->next->previous = temp;
		temp = temp->next;
		
	}
	DelayQueueTail = temp;
	KERNEL_SECONDS_TIME = 0;
	KERNEL_MINUTES_TIME = 0;
	KERNEL_HOURS_TIME = 0;
}

void AddDelayItem(int time, int wakeup, int delayedPCB_ID, MsgEnv* wakeupmsg)
{
	DelayQueueTail->time = time;
	DelayQueueTail->wakeup = wakeup;
	DelayQueueTail->delayedPCB_ID = delayedPCB_ID;
	DelayQueueTail->wakeupmsg = wakeupmsg;

	DelayQueueTail->next = DelayQueueHead;
	DelayQueueHead->previous = DelayQueueTail;
	DelayQueueTail->previous->next = NULL;

	DelayQueueHead = DelayQueueTail;
	DelayQueueTail = DelayQueueHead->previous;
	DelayQueueHead->previous = NULL;

}

void ClockHandler()
{
	mscount++;
	fflush(stdout);
	if (mscount == 10)
	{
		mscount = 0;
		KERNEL_SECONDS_TIME++; // need to add this 
		if (KERNEL_SECONDS_TIME == 60)
		{
			KERNEL_SECONDS_TIME = 0;
			KERNEL_MINUTES_TIME++;
			if (KERNEL_MINUTES_TIME == 60)
			{
				KERNEL_MINUTES_TIME = 0;
				KERNEL_HOURS_TIME++;
				if (KERNEL_HOURS_TIME == 24)
					KERNEL_HOURS_TIME = 0;
			}
		}
		
		WALL_CLOCK_SECONDS_TIME++; // need to add this 
		if (WALL_CLOCK_SECONDS_TIME == 60)
		{
			WALL_CLOCK_SECONDS_TIME = 0;
			WALL_CLOCK_MINUTES_TIME++;
			if (WALL_CLOCK_MINUTES_TIME == 60)
			{
				WALL_CLOCK_MINUTES_TIME = 0;
				WALL_CLOCK_HOURS_TIME++;
				if (WALL_CLOCK_HOURS_TIME == 24)
					WALL_CLOCK_HOURS_TIME = 0;
			}
		}
		//("Just chilling here, TIME: %i: %i: %i\n", KERNEL_HOURS_TIME, KERNEL_MINUTES_TIME, KERNEL_SECONDS_TIME);
		fflush(stdout);
		DelayItem* delayQ = DelayQueueHead;
		if (delayQ == NULL)
		{
		}
		while (delayQ->time != -1)
		{
			if (delayQ->time == 1)
			{
				delayQ->time = -1;

				if (delayQ != DelayQueueHead && delayQ != DelayQueueTail) 				
				{

					delayQ->previous->next = delayQ->next;
					delayQ->next->previous = delayQ->previous;				

					delayQ->previous = DelayQueueTail;
					DelayQueueTail->next = delayQ;

					DelayQueueTail = delayQ;
				}
				else if (delayQ == DelayQueueHead)
				{
					DelayQueueHead = delayQ->next;
					delayQ->next->previous = NULL;

					delayQ->previous = DelayQueueTail;
					DelayQueueTail->next = delayQ;

					DelayQueueTail = delayQ;
					
				}
				else
				{
					// do nothing, already at tail. BAM
				}
				
				

				snprintf(delayQ->wakeupmsg->message, MAXCHAR, "%i", delayQ->wakeup);
				delayQ->wakeupmsg->R_ID = delayQ->delayedPCB_ID;
				delayQ->wakeupmsg->S_ID = CLOCK->ID;
				delayQ->wakeupmsg->type = delayQ->wakeup;
				K_send(delayQ->delayedPCB_ID, delayQ->wakeupmsg);
			}
			delayQ->time--;
			delayQ = delayQ->next;
		}
		if (!(MSG_queue_isEmpty(PID_to_PCB(CLOCK_IPROCESS_PID)->message_queue)))
		{
			//MsgEnv* msg = K_receive();
			MsgEnv * msg = MSG_dequeue(PID_to_PCB(CLOCK_IPROCESS_PID)->message_queue);
			Add_TraceBufferRECEIVE(msg->S_ID, msg->R_ID, KERNEL_SECONDS_TIME, KERNEL_MINUTES_TIME, KERNEL_HOURS_TIME, msg->type);
snprintf(msg->message, MAXCHAR, "%c7%c[35m%c[ <0> ; <20> H%c[ <0> ; <60> f TIME:%i%i:%i%i:%i%i \n %c8", 0x1B, 0x1B, 0x1B, 0x1B, WALL_CLOCK_HOURS_TIME / 10, WALL_CLOCK_HOURS_TIME%10, WALL_CLOCK_MINUTES_TIME / 10, WALL_CLOCK_MINUTES_TIME%10, WALL_CLOCK_SECONDS_TIME/10, WALL_CLOCK_SECONDS_TIME%10, 0x1B);	
			//snprintf(msg->message, MAXCHAR, "%c7%c[ <0> ; <20> H%c[ <0> ; <60> f TIME:%i%i:%i%i:%i%i \n %c8", 0x1B, 0x1B, 0x1B, WALL_CLOCK_HOURS_TIME / 10, WALL_CLOCK_HOURS_TIME%10, WALL_CLOCK_MINUTES_TIME / 10, WALL_CLOCK_MINUTES_TIME%10, WALL_CLOCK_SECONDS_TIME/10, WALL_CLOCK_SECONDS_TIME%10, 0x1B);
			msg->R_ID = msg->S_ID;			
			msg->S_ID = CLOCK_IPROCESS_PID;
			msg->type = MSG_TIME;
			K_send(msg->R_ID, msg);
		}
	}
	/*
	if ( !(Queue_isEmpty(CLOCK->message_queue) ) )
	{
		MSG* msg = MSG_dequeue(CLOCK->message_queue);
		msg->R_ID = msg->S_ID;
		msg->S_ID = CLOCK->ID;
		k_send_message(msg, msg->S_ID);
	}*/	
}

void cleanup()
{

	// terminate child process(es)
	int status;
	
	kill(kb_pid,SIGINT);
	
	wait(&status);
	
	printf("Confirmation that KB has exited\n");
	
	kill(crt_pid, SIGINT);
	
	wait(&status);
	
	printf("Confirmation that CRT has exited\n");

	// remove shared memory segment and do some standard error checks
	status = munmap(in_mem_p1, buffersize);
	if (status == -1){
		printf("Bad munmap of KB during cleanup\n");
	}
	
	status = munmap(out_mem_p1, buffersize);
	if (status == -1){
		printf("Bad munmap of CRT during cleanup \n");
	}
	// close the temporary mmap file 
	status = close(kbfid); 
	if (status == -1){
		printf("Bad close of temporary KB mmap file during cleanup\n");
	}
	status = close(crtfid);
	if (status == -1){
		printf("Bad close of temporary CRT mmap file during cleanup \n");
	}
	// unlink (i.e. delete) the temporary mmap file
	status = unlink(KBFileName); 
	if (status == -1){
		printf("Bad unlink of KB during cleanup.\n");
	}
	
	status = unlink(crtFileName);
	if (status == -1){
		printf("Bad unlink of CRT during cleanup. \n");
	}
	
	main_cleanup();
	
}

int SegFault()
{
	printf("Segmentation fault detected");
	exit(0);
}

int KeyboardHandler()
{
	
	// copy input buffer
	
	if (in_mem_p->indata[0] != '\0')
	{
		
	    // we should parse the input string and execute the command given,
	    //  but for now we just echo the input
	    // 
		in_mem_p1->ok_flag = 0;  // tell child that the buffer has been emptied
		KB = PCBAddress[KB_IPROCESS_PID-1];
		
		if ( MSG_queue_isEmpty(KB->message_queue)==0 )
		{
			fflush(stdout);
			//MsgEnv* receive = K_receive();
			MsgEnv* receive = MSG_dequeue(KB->message_queue);
			Add_TraceBufferRECEIVE(receive->S_ID, receive->R_ID, KERNEL_SECONDS_TIME, KERNEL_MINUTES_TIME, KERNEL_HOURS_TIME, receive->type);	
			receive->R_ID = receive->S_ID;
			receive->S_ID = KB->ID;
			receive->type = KEYBOARD_INPUT;
			fflush(stdout);
			strcpy(receive->message,in_mem_p1->indata); 
			K_send( receive->R_ID, receive);
		}
		else {
		}
	}
}

int CRTHandler()
{
	//message was successfully sent
	//will send message back to requesting process
	
	//peak at message
	if (! (MSG_queue_isEmpty(CRT->message_queue)))
	{
		MsgEnv* receive = CRT->message_queue->head;
		if (receive->type != WAITING_CRT)
		{
			strcpy(out_mem_p1->indata, receive->message);
			receive->type = WAITING_CRT;
			out_mem_p1->ok_flag = 1;
		}
		else {
			//MsgEnv* receive = K_receive();			
			MsgEnv* receive = MSG_dequeue(CRT->message_queue);
			Add_TraceBufferRECEIVE(receive->S_ID, receive->R_ID, KERNEL_SECONDS_TIME, KERNEL_MINUTES_TIME, KERNEL_HOURS_TIME, receive->type);
			receive->R_ID = receive->S_ID;
			receive->S_ID = CRT->ID;
			receive->type = RETURN_ACK;
			K_send( receive->R_ID, receive);
		}
	}		
	
	//should never output before we have received a message in
	
}

int Terminate()
{
	cleanup();
	printf("RTX Kernel Terminating\n");
	fflush(stdout);	
	curses_terminate();	
	exit(0);
	;
}
