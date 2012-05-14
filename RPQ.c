//INCLUDE LIBs
#include <stdlib.h>

//INCLUDE HEADERS
#include "Constants.h"
#include "Structures.h"
#include "RPQ.h"

#include <stdio.h>


/*FUNCTION IMPLEMENTATION
/*---------------------*/


RPQ_queue* RPQ_create()
{
	//Create Ready Process Queue
	RPQ_queue* newQ = NULL;
	newQ = (RPQ_queue*)malloc(sizeof(RPQ_queue));
	if(newQ != NULL)
	{
		//Create a Queue for Each Priority Level
		newQ->P0 = PCB_queue_create();
		newQ->P1 = PCB_queue_create();
		newQ->P2 = PCB_queue_create();
		newQ->P3 = PCB_queue_create();
		//Add a pointer to the NULL Process
		newQ->NP = PID_to_PCB(NULL_PROCESS);
		//Check they all created successfully
		if(newQ->P0 == NULL || newQ->P1 == NULL || newQ->P2 == NULL || newQ->P3 == NULL)
		{	
			//Terminate RPQ being created
			RPQ_terminate(newQ);
			newQ = NULL;
		}
	}
	return newQ;
}

void RPQ_terminate(RPQ_queue* Q)
{
	if(Q != NULL)
	{
		//Free memory for queues
		free(Q->P0);
		free(Q->P1);
		free(Q->P2);
		free(Q->P3);
		//Free memory RPQ
		free(Q);
	}
}

int RPQ_enqueue(RPQ_queue* Q, PCB* PCBpointer)
{
	//Error Check
	if(Q == NULL || PCBpointer == NULL || PCBpointer->priority < 0 || (PCBpointer->priority > 3 && PCBpointer->priority != NULL_PRIORITY))
	{
		if (Q == NULL)
			printf("Queue is NULL");
		if (PCBpointer == NULL)
			printf("PCBPointer is NULL");
		if (PCBpointer->priority < 0)
			printf("Invalid Priority");
		if (PCBpointer->priority > 3);
			printf("Invalid Priority2");
		fflush(stdout);
		return ERROR;
	}
	//Enqueue onto priority queue
	if(PCBpointer->priority == 0)
		return PCB_enqueue(Q->P0, PCBpointer);
	else if(PCBpointer->priority == 1)
		return PCB_enqueue(Q->P1, PCBpointer);
	else if(PCBpointer->priority == 2)
	{
		return PCB_enqueue(Q->P2, PCBpointer);
	}
	else if(PCBpointer->priority == 3)
	{
		return PCB_enqueue(Q->P3, PCBpointer);
	}
	else {
		Q->NP = PCBpointer;
		
	}

}

PCB* RPQ_dequeue(RPQ_queue* Q)
{	
	//Error Check
	if(Q == NULL)
		return NULL;
	//Dequeue from highest priority first
	if(PCB_queue_isEmpty(Q->P0) == 0)
		return PCB_dequeue(Q->P0);
	else if(PCB_queue_isEmpty(Q->P1) == 0)
		return PCB_dequeue(Q->P1);
	else if(PCB_queue_isEmpty(Q->P2) == 0)
		return PCB_dequeue(Q->P2);
	else if(PCB_queue_isEmpty(Q->P3) == 0)
	{
		return PCB_dequeue(Q->P3);
	}
	//Nothing in all queues, return NULL process
	else 
	{
		return Q->NP;
	}
}

PCB* RPQ_dequeue_PID(RPQ_queue* Q, int PID)
{	
	//Error Check
	if(Q == NULL || PID_to_PCB(PID) == NULL || PID != NULL_PROCESS)
		return NULL;

	//Convert PID to PCB pointer
	PCB* tempPCB = PID_to_PCB(PID);

	//Dequeue specific PCB from its priority level queue, 
	//if not in queue, NULL will inherently be returned
	if(tempPCB->priority == 0)
		return PCB_dequeue_PID(Q->P0, PID);
	if(tempPCB->priority == 1)
		return PCB_dequeue_PID(Q->P1, PID);
	if(tempPCB->priority == 2)
		return PCB_dequeue_PID(Q->P2, PID);
	if(tempPCB->priority == 3)
		return PCB_dequeue_PID(Q->P3, PID);
}



