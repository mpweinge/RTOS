 /*
 *  TraceBuffer.c
 *  
 *
 *  Created by Michael Weingert on 11-11-14.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TraceBuffer.h"
#include "Constants.h"
#include "Structures.h"

#include <stdio.h>

#include <string.h>

void InitializeTraceBufferSEND()
{
	int i;
	
	TraceBuffer* temp;
	TraceBuffer* tempPrev;
	
	temp = (TraceBuffer *)malloc(sizeof(TraceBuffer));
	headSEND = temp;
	temp->previous = NULL;
	tempPrev = temp;
	
	for (i = 1; i < TRACE_BUFFER_NUM; i++)
	{
		temp->S_ID = -1; // for sanity checks
		temp->next = (TraceBuffer *)malloc(sizeof(TraceBuffer));
		tempPrev = temp;		
		temp = temp->next;
		temp->previous = tempPrev;
	}
	tailSEND = tempPrev;
}

void InitializeTraceBufferRECEIVE()
{
	int i;
	
	TraceBuffer* temp;
	TraceBuffer* tempPrev;
	
	temp = (TraceBuffer *)malloc(sizeof(TraceBuffer));
	headRECEIVE = temp;
	temp->previous = NULL;
	tempPrev = temp;	

	for (i = 1; i < TRACE_BUFFER_NUM; i++)
	{
		temp->S_ID = -1; // for sanity checks
		temp->next = (TraceBuffer *)malloc(sizeof(TraceBuffer));
		tempPrev = temp;		
		temp = temp->next;
		temp->previous = tempPrev;
	}
	tailRECEIVE = tempPrev;
}

void DeallocateTraceBufferSEND()
{
	TraceBuffer* temp = headSEND;
	while (temp->next != NULL)
	{
		temp = temp->next;
		free(temp->previous);
	}		
	free(temp);
	headSEND = NULL;
	tailSEND = NULL;
}

void DeallocateTraceBufferRECEIVE()
{
	TraceBuffer* temp = headRECEIVE;
	while (temp->next != NULL)
	{
		temp = temp->next;
		free(temp->previous);
	}	
	free(temp);
	headRECEIVE = NULL;
	tailRECEIVE = NULL;
}

void Add_TraceBufferSEND(int S_ID, int R_ID, int TimeStampSec, int TimeStampMin, int TimeStampHr, int Message_Type)
{
	tailSEND->S_ID = S_ID;
	tailSEND->R_ID = R_ID;
	tailSEND->TimeStampSec = TimeStampSec;
	tailSEND->TimeStampMin = TimeStampMin;
	tailSEND->TimeStampHr = TimeStampHr;
	tailSEND->Message_Type = Message_Type;


	tailSEND->next = headSEND;
	headSEND->previous = tailSEND;
	tailSEND->previous->next = NULL;
	
	headSEND = tailSEND;
	tailSEND = headSEND->previous;
	headSEND->previous = NULL;

}

void Add_TraceBufferRECEIVE(int S_ID, int R_ID, int TimeStampSec, int TimeStampMin, int TimeStampHr, int Message_Type)
{	
	tailRECEIVE->S_ID = S_ID;
	tailRECEIVE->R_ID = R_ID;
	tailRECEIVE->TimeStampSec = TimeStampSec;
	tailRECEIVE->TimeStampMin = TimeStampMin;
	tailRECEIVE->TimeStampHr = TimeStampHr;
	tailRECEIVE->Message_Type = Message_Type;
	tailRECEIVE->next = headRECEIVE;
	headRECEIVE->previous = tailRECEIVE;
	tailRECEIVE->previous->next = NULL;
	
	headRECEIVE = tailRECEIVE;
	tailRECEIVE = headRECEIVE->previous;
	headRECEIVE->previous = NULL;
}

void PrintTraceBufferSEND(MsgEnv* message)
{
	int i;
	char tempMessage[MAXCHAR];
	// to do later
	TraceBuffer * temp;
	temp = headSEND;
	
	snprintf(message->message, MAXCHAR, "%c[33mSEND TRACE BUFFER\n%c[0m", 0x1B, 0x1B);	
	//strcat(message->message, "SEND TRACE BUFFER\n");
	if (temp->S_ID == -1 )
	{
		snprintf(message->message, MAXCHAR, "NULL SEND TRACE BUFFER");
		return;
	}
	//message->message = ""; //NOTE NEED TO CHECK STRCAT OPERATION
	for (i = 1; i < TRACE_BUFFER_NUM; i++)
	{
		if (temp->S_ID == -1)
			break;
		snprintf(tempMessage, MAXCHAR, "S_ID %i R_ID %i MessageType %i Time %i : %i : %i\n", temp->S_ID, temp->R_ID, temp->Message_Type, temp->TimeStampHr, temp->TimeStampMin, temp->TimeStampSec);
		strcat(message->message, tempMessage);
		//message->message += temp->S_ID + temp->R_ID + temp->Message_Type + temp->TimeStampHr + ":" + temp->TimeStampMin + ":" + temp->TimeStampSec;// etc etc
		if (temp->next == NULL)
			break;
		temp = temp->next;
	}
	char command[40];
	snprintf(command, 40, "%c[37mRECEIVE TRACE BUFFER\n%c[0m", 0x1B, 0x1B);

	strcat(message->message, command);
	temp = headRECEIVE;
	if (temp->S_ID == -1 )
	{
		snprintf(message->message, MAXCHAR, "NULL RECEIVE TRACE BUFFER");
		return;
	}
	//msg->message = ""
	for (i = 1; i < TRACE_BUFFER_NUM; i++)
	{
		if (temp->S_ID == -1)
			break;
		snprintf(tempMessage, MAXCHAR, "S_ID %i R_ID %i MessageType %i Time %i : %i : %i\n", temp->S_ID, temp->R_ID, temp->Message_Type, temp->TimeStampHr, temp->TimeStampMin, temp->TimeStampSec);
		strcat(message->message, tempMessage);
		//msg->message += temp->S_ID + temp->R_ID + temp->Message_Type + temp->TimeStampHr + ":" + temp->TimeStampMin + ":" + temp->TimeStampSec;// etc etc
		if (temp->next == NULL)
			break;
		temp = temp->next;
	}
}

void PrintTraceBufferRECEIVE(MsgEnv* msg)
{
	int i;
 // to do later
	char * tempMessage = (char *)malloc(MAXCHAR * sizeof(char));
	TraceBuffer * temp;
	temp = headRECEIVE;
	if (temp->S_ID == -1 )
	{
		snprintf(msg->message, sizeof(msg->message), "NULL TRACE BUFFER");
		return;
	}
	//msg->message = ""
	for (i = 1; i < TRACE_BUFFER_NUM; i++)
	{
		if (temp->S_ID == -1)
			break;
		snprintf(tempMessage, MAXCHAR, "S_ID %i R_ID %i MessageType %i Time %i : %i : %i\n", temp->S_ID, temp->R_ID, temp->Message_Type, temp->TimeStampHr, temp->TimeStampMin, temp->TimeStampSec);
		strcat(msg->message, tempMessage);
		//msg->message += temp->S_ID + temp->R_ID + temp->Message_Type + temp->TimeStampHr + ":" + temp->TimeStampMin + ":" + temp->TimeStampSec;// etc etc
		if (temp->next == NULL)
			break;
		temp = temp->next;
	}
}
	
	
	
