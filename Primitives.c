#include "Primitives.h"
#include "Structures.h"
#include <stdlib.h>
#include "RTX-G15.h"
#include "Interrupt.h"
#include "Constants.h"
#include "RPQ.h"
#include "KernelPointers.h"
#include <execinfo.h>
#include "TraceBuffer.h"
#include <signal.h>
#include <stdio.h>

int return_code;
MsgEnv* tempMSG;

int K_send(int target_pid, MsgEnv* message)
{
	message->R_ID = target_pid;
	PCB* target_PCB;
	target_PCB = PID_to_PCB(target_pid);
	if ( message == NULL || target_PCB == NULL)
 	 	return -1;					
    MSG_enqueue(target_PCB->message_queue, message);
	if (target_PCB->state == BLOCKED_RECIEVE) {		
	   target_PCB->state = READY;
      RPQ_enqueue(RPQ, target_PCB);
	}  
	Add_TraceBufferSEND(message->S_ID, message->R_ID, KERNEL_SECONDS_TIME, KERNEL_MINUTES_TIME, KERNEL_HOURS_TIME, message->type);						
	return 0;
}

MsgEnv* K_receive()
{
	if (curr_proc1->priority == I_PROCESS && MSG_queue_isEmpty(curr_proc1->message_queue))
		return NULL;
	if (MSG_queue_isEmpty(curr_proc1->message_queue)) {	
		curr_proc1->state = BLOCKED_RECIEVE;	
		return_code = 0;
		process_switch();				
	}
	tempMSG = MSG_dequeue(curr_proc1->message_queue);
	Add_TraceBufferRECEIVE(tempMSG->S_ID, tempMSG->R_ID, KERNEL_SECONDS_TIME, KERNEL_MINUTES_TIME, KERNEL_HOURS_TIME, tempMSG->type);	
	return tempMSG;
}

MsgEnv* K_request_envelope()
{
 	if (curr_proc1->priority == I_PROCESS && MSG_queue_isEmpty(Free_Msg_Queue))
 		return NULL;
 	while (MSG_queue_isEmpty(Free_Msg_Queue)) {
		curr_proc1->state = BLOCKED_ENV_REQUEST;		
		PCB_enqueue(Block_Env, curr_proc1);	
		return_code = 0;
		process_switch();				
	}
	//("Size of message queue %i", Free_Msg_Queue->size);
	//fflush(stdout);	
	tempMSG = MSG_dequeue(Free_Msg_Queue);		
 	return tempMSG;
}

int K_release_msg_env(MsgEnv* message)
{
	if (message == NULL)
	{
		printf("\nFree NULL Message");
		fflush(stdout);
		return -1;
	}
	message->S_ID = -1;
	message->R_ID = -1;
	snprintf(message->message, MAXCHAR, "");
	message->type = -1;
	MSG_enqueue(Free_Msg_Queue, message);
	if (PCB_queue_isEmpty(Block_Env) == 0) 	{	// indicating it is not empty	
		PCB* tempPCB = PCB_dequeue(Block_Env);
		tempPCB->state = READY;
		RPQ_enqueue(RPQ, tempPCB);
		//printf("Enqueued PCB %i\n", tempPCB->ID);
		//fflush(stdout);		
	}
	return 0;
}

int K_release_processor()
{
	if (curr_proc1->priority == I_PROCESS)
		return -1;
	curr_proc1->state = READY;
	RPQ_enqueue(RPQ, curr_proc1);
	return_code = 0;
   process_switch();
	return 0;
}

int K_request_process_status(MsgEnv* message)
{
	if (message == NULL)
		return -1;
	char tempMessage[MAXCHAR];
	snprintf(message->message, MAXCHAR, "\nThere are %i processes in existance \n", NUM_PROCESSES);
	int i = 0;
	for (i; i < NUM_PROCESSES; i++) {
		snprintf(tempMessage, MAXCHAR, "\nID: %i State: %i Priority: %i", PCBAddress[i]->ID, PCBAddress[i]->state, PCBAddress[i]->priority);
		strcat(message->message, tempMessage);
	}
	return 0;
}

int K_change_priority(int new_priority, int target_pid)
{
	if (new_priority < 0 || new_priority > 3 || target_pid < 0 || target_pid > NUM_PROCESSES)
	{
		printf("Invalid input");
		fflush(stdout);	
		return -1;
	}
	PCB* target_PCB;
	target_PCB = PID_to_PCB(target_pid);
	if (target_PCB->priority == I_PROCESS)
 		return -1;
 	if (target_PCB->state != READY)		// includes possibility of current process being target_pid
		target_PCB->priority = new_priority;
	else {	 	// the target process is within the ready queue
		PCB* tempPCB = RPQ_dequeue_PID(RPQ, target_PCB->ID);
		tempPCB->priority = new_priority;
		RPQ_enqueue(RPQ, tempPCB);
	}
	return 0;
}

int K_request_delay(int time_delay, int wakeup_code, MsgEnv* message)
{
	if (time_delay <= 0 || message == NULL)
		return -1;
	AddDelayItem(time_delay, wakeup_code, message->S_ID, message);
	return 0;
}

int K_send_console_chars(MsgEnv* message)
{
	message->R_ID = CRT_IPROCESS_PID;
	K_send( CRT_IPROCESS_PID, message);
	interruptHand(SIGUSR2);
}

int K_get_console_chars(MsgEnv* message)
{
	message->R_ID = KB_IPROCESS_PID;
	K_send( KB_IPROCESS_PID, message);
}

void K_terminate()
{
	Terminate();
}

void context_switch(jmp_buf prev, jmp_buf next)
{
	/*if (curr_proc1->ID != 9)
	{
		printf("Long jumped like a boss? CURRPROC ID: %i, RETURN:%i", curr_proc1->ID, return_code);
		fflush(stdout);
		//curr_proc1->main();
		//curr_proc1 = PID_to_PCB(return_code);
		//curr_proc1->state = EXECUTING;
		void* callstack[128];
		int i, frames = backtrace(callstack, 128);
		char** strs = backtrace_symbols(callstack, frames);
		for (i = 0; i < frames; ++i) {
			printf("%s\n", strs[i]);
		}
		free(strs);

	}*/

	//store the current context of prev running PCB into the jmp_buf
	return_code = setjmp(prev);
	//if setjmp was just called and context was just stored, if statement is true
	//if returning from longjmp call to resume running, if statement is false and program continues to run

	//printf("\nReturn Code %i\nCurr_Proc %i", return_code, curr_proc1->ID);
	/*if (return_code != 0 && curr_proc1->ID != 9)
	{
		printf("Long jumped like a boss? CURRPROC ID: %i, RETURN:%i  ", curr_proc1->ID, return_code);
		fflush(stdout);
		//curr_proc1->main();
		//curr_proc1 = PID_to_PCB(return_code);
		//curr_proc1->state = EXECUTING;
		void* callstack[128];
		int i, frames = backtrace(callstack, 128);
		char** strs = backtrace_symbols(callstack, frames);
		for (i = 0; i < frames; ++i) {
			printf("%s\n", strs[i]);
		}
		free(strs);

	}*/
	if(return_code == 0)
	{
		if (curr_proc1->ID == 4)
		{
		//	printf("clock is IN THE GAME");
		//	fflush(stdout);
		}
		//restore the context of next to run PCB //store curr_proc ID
		//longjmp(next,curr_proc1->ID);
		longjmp(next, 1);
	}
	//printf("Exiting Context Switch. This is good %i", return_code);
	//fflush(stdout);
}

void process_switch()
{
	//printf("Arriving Here CODE: %i", return_code);
	//fflush(stdout);
	
	if (curr_proc1->ID == 8)
	{
		//printf("Process Switching, ID: %i", curr_proc1->ID);
		fflush(stdout);
	}
	
	if (return_code == 0) // temp fix, look into fucking stupid context switch return
	{
	
		//Set pointers for PCBs
		//Store current
		PCB* prev = curr_proc1;
		//Get next and store
		PCB* next = RPQ_dequeue(RPQ);
	
	
		//For debugging
		//printf("Next Process %i", next->ID);
		//fflush(stdout);
	
		//Set status of next
		next->state = EXECUTING;
		//set current to next
		curr_proc1 = next;
		
		if (next->ID == 4)
		{
		//	printf("SWITCHING FROM %i to %i", prev->ID, next->ID);
		//	fflush(stdout);
			//curr_proc1->ID = 4;
		}
		
		//Switch the context 
		context_switch(*(prev->context), *(curr_proc1->context));
	}
	/*if(curr_proc1->ID != 9)
	{
		printf("Something is wrong ... \n \n");
		fflush(stdout);
	}*/
	/*if (return_code != 9)
	{
		printf("Exiting Process_Switch. This is really good, CURR_ID:%i", curr_proc1->ID);
		fflush(stdout);
	}*/
}

void atomic(int onoff) //look into this later
{
	//static sigset_t oldmask;
 	sigset_t newmask;

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM); // the alarm signal
  	sigaddset(&newmask, SIGINT);  // the CNTRL-C
  	sigaddset(&newmask, SIGUSR1); // the CRT signal
  	sigaddset(&newmask, SIGUSR2); // the KB signal

 	if (onoff == ON)
 	{
		//oldmask = siggetmask();
  		sigprocmask(SIG_BLOCK, &newmask, NULL);
 	}
 	else
 	{
  		// unblock the signals
  		sigprocmask(SIG_UNBLOCK, &newmask, NULL);
		//printf("Exiting Atomic Off");
		//fflush(stdout);
 	}
}


