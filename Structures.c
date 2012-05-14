//INCLUDE LIBs
#include <stdlib.h>
#include <stdio.h>

//INCLUDE HEADERS
#include "Constants.h"
#include "Structures.h"
#include "KernelPointers.h"


/*FUNCTION IMPLEMENTATION
/*---------------------*/

PCB_queue* PCB_queue_create()
{
	//Create PCB_queue
	PCB_queue* newQ = NULL;
	newQ = (PCB_queue*)malloc(sizeof(PCB_queue)); 		//CHECK SIZE OUTPUT
	if(newQ != NULL)
	{
		//Set Values
		newQ->head = NULL;
		newQ->tail = NULL;
		newQ->size = 0;
	}
	//Return PCB_queue
	return newQ;
}

MSG_queue* MSG_queue_create()
{
	//Create MSG_queue
	MSG_queue* newQ = NULL;
	newQ = (MSG_queue*)malloc(sizeof(PCB_queue)); 		//CHECK SIZE OUTPUT
	if(newQ != NULL)
	{
		//Set Values
		newQ->head = NULL;
		newQ->tail = NULL;
		newQ->size = 0;
	}
	//Return MSG_queue
	return newQ;
}

void PCB_queue_terminate(PCB_queue* Q)
{
	if(Q != NULL)
		//Release memory
		free(Q);
}

void MSG_queue_terminate(MSG_queue* Q)
{
	if(Q != NULL)
		//Release memory
		free(Q);
}

int PCB_enqueue(PCB_queue* Q, PCB* PCBpointer)
{
	//Error Check
	if(Q == NULL || PCBpointer == NULL)
		return ERROR;
	//Add PCB
	if(Q->size == 0)
	{
		Q->head = PCBpointer;
		Q->tail = PCBpointer;
	}
	else
	{
		Q->tail->next = PCBpointer;
		Q->tail = PCBpointer;
	}
	//Set next of last PCB to NULL
	PCBpointer->next = NULL;
	Q->size ++;
	return SUCCESS;
}

int MSG_enqueue(MSG_queue* Q, MsgEnv* MSGpointer)
{
	//Error Check
	if(Q == NULL || MSGpointer == NULL)
		return ERROR;
	//Add MSG
	if(Q->size == 0)
	{
		Q->head = MSGpointer;
		Q->tail = MSGpointer;
	}
	else
	{
		Q->tail->next = MSGpointer;
		Q->tail = MSGpointer;
	}
	//Set next of last MSG to NULL
	MSGpointer->next = NULL;
	Q->size ++;
	return SUCCESS;
}

PCB* PCB_dequeue(PCB_queue* Q)
{
	//Error Check
	if(Q == NULL || Q->size == 0)
		return NULL;
	
	//Set temp to head
	PCB* tempPCB = Q->head;
	
	//Remove PCB	
	if(Q->size == 1)
	{
		Q->head = NULL;
		Q->tail = NULL;
	}
	else
		Q->head = Q->head->next;

	//Set next of returning item to NULL	
	tempPCB->next = NULL;
	Q->size --;
	return tempPCB;	
}

//ADDED FUNCTION TO FILE---------------------------------------------------------------------
PCB* PCB_dequeue_PID(PCB_queue* Q, int PID)
{
	//Error Check
	if(Q == NULL || Q->size == 0)
		return NULL;
	
	//Create temp pointers
	PCB* tempPCB = Q->head;
	PCB* prevPCB = Q->head;

	//Traverse List to Find PID
	while(tempPCB)
	{
		//If PID found
		if(tempPCB->ID == PID)
		{
			if(Q->size == 1)
			{
				Q->head = NULL;
				Q->tail = NULL;
			}
			else
			{
				prevPCB->next = tempPCB->next;
				if(Q->tail == tempPCB)
					Q->tail = prevPCB;
			}
			//Set next of returning item to NULL
			tempPCB->next = NULL;
			Q->size --;
			//Return PCB
			return tempPCB;	
		}
		//Move to next node
		prevPCB = tempPCB;
		tempPCB = tempPCB->next;
	}
	//If not found, return NULL
	return NULL;
}
//-------------------------------------------------------------------------------------------


MsgEnv* MSG_dequeue(MSG_queue* Q)
{
	//Error Check
	if(Q == NULL || Q->size == 0)
		return NULL;
	
	//Set temp to head
	MsgEnv* tempMSG = Q->head;
		
	//Remove MSG
	if(Q->size == 1)
	{
		Q->head = NULL;
		Q->tail = NULL;
	}
	else
		Q->head = Q->head->next;

	//Set next of returning item to NULL
	tempMSG->next = NULL;
	Q->size --;
	return tempMSG;	
}

int PCB_queue_isEmpty(const PCB_queue* Q)
{
	if(Q == NULL)
		return ERROR;
	if(Q->size == 0)
		//Return 1 (True) if empty
		return 1;
	else
		return 0;
}

int MSG_queue_isEmpty(const MSG_queue* Q)
{
	if(Q == NULL)
		return ERROR;
	if(Q->size == 0)
		//Return 1 (True) if empty
		return 1;
	else
		return 0;
}

PCB* PID_to_PCB(const int PID)
{
	//Error Check
	if(PID > NUM_PROCESSES)
		return NULL;
	
	//0 indexed array
	return PCBAddress[PID-1];
}
