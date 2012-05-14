//INCLUDE LIBs
#include <stdlib.h>
#include <stdio.h>

//INCLUDE HEADERS
#include "Constants.h"
#include "Structures.h"
#include "KernelPointers.h"
#include "Initialize.h"
#include "RPQ.h"
#include "CLOCK.h"
#include "ProcessA.h"
#include "ProcessB.h"
#include "ProcessC.h"
#include "NullProcess.h"
#include "CCI.h"

#include <setjmp.h>


char * jmpsp;

/*FUNCTION IMPLEMENTATION
/*---------------------*/

int CreateInitializationTable()
{
	initTable1 = (initTable*)malloc(NUM_PROCESSES * sizeof(initTable));
	if (initTable1 == NULL)
		return ERROR;
/* Note that iProcesses have no main function, and do not perform context switching */
	//Set Data for process 1
	initTable1[0].ID = KB_IPROCESS_PID;
	initTable1[0].state = READY;
	initTable1[0].priority = I_PROCESS;
	initTable1[0].StackSize = STACKSIZE;

	//Set Data for process 2
	initTable1[1].ID = CRT_IPROCESS_PID;
	initTable1[1].state = READY;
	initTable1[1].priority = I_PROCESS;
	initTable1[1].StackSize = STACKSIZE;
	
	//Set Data for process 3
	initTable1[2].ID = CLOCK_IPROCESS_PID;
	initTable1[2].state = READY;
	initTable1[2].priority = I_PROCESS;
	initTable1[2].StackSize = STACKSIZE;

	//Set Data for process 4
	initTable1[3].ID = CLOCK_PID;
	initTable1[3].state = READY;
	initTable1[3].priority = 1;
	initTable1[3].StackSize = STACKSIZE;
	initTable1[3].main = &ClockMain;

	//Set Data for process 5
	initTable1[4].ID = USER_A;
	initTable1[4].state = READY;
	initTable1[4].priority = 2;
	initTable1[4].StackSize = STACKSIZE;
	initTable1[4].main = &ProcessA;

	//Set Data for process 6
	initTable1[5].ID = USER_B;
	initTable1[5].state = READY;
	initTable1[5].priority = 2;
	initTable1[5].StackSize = STACKSIZE;
	initTable1[5].main = &ProcessB;

	//Set Data for process 7
	initTable1[6].ID = USER_C;
	initTable1[6].state = READY;
	initTable1[6].priority = 2;
	initTable1[6].StackSize = STACKSIZE;
	initTable1[6].main = &ProcessC;

	//Set Data for process 8
	initTable1[7].ID = CCI_PROCESS_PID;
	initTable1[7].state = READY;
	initTable1[7].priority = 1;
	initTable1[7].StackSize = STACKSIZE;
	initTable1[7].main = &CCI;

	//Set Data for process 9
	initTable1[8].ID = NULL_PROCESS;
	initTable1[8].state = READY;
	initTable1[8].priority = NULL_PRIORITY;
	initTable1[8].StackSize = STACKSIZE;
	initTable1[8].main = &NullProcess;

	return SUCCESS;
}

int Initialize_MSG_Queues()
{
	Free_Msg_Queue = MSG_queue_create();
	if (Free_Msg_Queue == NULL)
	{
		printf("Error creating Free Messsage Queue");
		return ERROR;
	}
		
	MsgEnv* tempMessage = NULL;
	int i;	
	for (i = 0; i < NUM_MESSAGES; i++)				//REMOVE -1 and TEST
	{
		tempMessage = (MsgEnv*)malloc(sizeof(MsgEnv));
		if (tempMessage == NULL)
		{
			printf("Error allocating memory for Message");
			return ERROR;
		}
		//put on free message queue
		if (MSG_enqueue(Free_Msg_Queue, tempMessage) == -1)
		{
			printf("Error adding message to free message queue");
			return ERROR;
		}
	}
	return SUCCESS;
}

int Initialize_PCB_Queues()
{
	Block_Env = PCB_queue_create();
	if (Block_Env == NULL)
	{
		printf("Error creating Blocked on Env queue");
		return ERROR;
	}

	/*CANT DECLARE VARIABLE IN SCOPE OF FUNCTION JOHN*/
	RPQ = RPQ_create();
	if(RPQ == NULL)
	{
		printf("Error allocating memory for RPQ queue");
		return ERROR;
	}

	return SUCCESS;
}

int Initialize_PCBs()
{
	int i;
	PCB* temp;
	//Creates PCB and sets values
	for (i = 0; i < NUM_PROCESSES; i++)
	{

		temp = (PCB*) malloc(sizeof(PCB));
		if (temp == NULL)
		{
			printf("Error allocating memory for PCB");
			return ERROR;
		}
		temp->ID = initTable1[i].ID;
		temp->state = initTable1[i].state;
		temp->priority = initTable1[i].priority;
		temp->stack = ((char*)malloc(initTable1[i].StackSize))+initTable1[i].StackSize-8; //to be safe
		temp->context = (jmp_buf*)malloc(sizeof(jmp_buf));
		temp->main = initTable1[i].main;
		if(temp->stack == NULL)
		{
			printf("Error allocating memory for Stack");
			return ERROR;
		}		

		temp->message_queue = MSG_queue_create();
		if (temp->message_queue == NULL)
		{
			printf("Error allocating memory for MSG queue");
			return ERROR;
		}
		PCBAddress[i] = temp;
	}
	
	//sets Context for each PCB
	int PID_context = 4;
	PCB* nextPCB = PID_to_PCB(PID_context);
	
	do
	{
		if(setjmp(kernel_buf)==0)
		{
			
			jmpsp = nextPCB->stack;			

			//#ifdef i386
				__asm__ ("movl %0,%%esp" :"=m" (jmpsp)); // if Linux i386 target
			//#endif // line 2
			//#ifdef __sparc
			//	_set_sp(nextPCB->stack); // if Sparc target (eceunix)
			//#endif

			
			
			int l = setjmp(*(nextPCB->context));
			if(l==0)
			{
				longjmp(kernel_buf,1);
			}
			else
			{
				curr_proc1->state = EXECUTING;
				curr_proc1->main();
			}
			
		}
		nextPCB = PID_to_PCB(++PID_context);
	}
	while(PID_context <= NUM_PROCESSES);
	
	//Range on the for-loop will avoid enqueue call for I_Processes and Null Processes
	for (i = 4; i <= NUM_PROCESSES; i++)
	{
		PCB* temp = PID_to_PCB(i);
		if(RPQ_enqueue(RPQ, temp)==-1)
		{
			printf("Error Putting PCB onto RPQ");
			return ERROR;
		}
	}
	return SUCCESS;
}
