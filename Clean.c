//INCLUDE LIBs
#include <stdlib.h>
#include <stdio.h>

//INCLUDE HEADERS
#include "Clean.h"
#include "Structures.h"
#include "KernelPointers.h"
#include "RPQ.h"
#include "TraceBuffer.h"


/*FUNCTION IMPLEMENTATION
/*---------------------*/

void main_cleanup()
{
	//Clean Init Table
	if(initTable1 != NULL)
		free(initTable1);

	//Clean Message Queue
	if(Free_Msg_Queue != NULL)
	{
		MsgEnv* temp;
		while( (temp = MSG_dequeue(Free_Msg_Queue) ) != NULL)
			free(temp);
		free(Free_Msg_Queue);
	}
	
	//Clean Blocked Env Queue
	if(Block_Env != NULL)
		free(Block_Env);
	
	//Clean RPQ
	if(RPQ != NULL)
	{
		if(RPQ->P0 != NULL)
			free(RPQ->P0);
		if(RPQ->P1 != NULL)
			free(RPQ->P1);
		if(RPQ->P2 != NULL)
			free(RPQ->P2);
		if(RPQ->P3 != NULL)
			free(RPQ->P3);
	}
	
	int i;
	int switchMsg;
	switchMsg = 1;
	
	for (i = 0; i < NUM_PROCESSES; i++)
	{
		PCB* tempPCB = PCBAddress[i];
		if(tempPCB != NULL)
		{

			MsgEnv* tempMSG;
			if(tempPCB->message_queue != NULL)
			{
				while (switchMsg)
				{
					tempMSG = MSG_dequeue(tempPCB->message_queue);				
					if (tempMSG != NULL)
						free(tempMSG);
					else
						switchMsg = 0;
				}

				free(tempPCB->message_queue);
			}
			//if(tempPCB->stack != NULL)
			//	free(tempPCB->stack);
			free(tempPCB);
		}
	}

	//DeallocateTraceBufferSEND();
	//DeallocateTraceBufferRECEIVE();
}

