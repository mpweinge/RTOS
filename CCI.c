/*
 *  CCI.c
 *  
 *
 *  Created by Michael Weingert on 11-11-14.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <string.h>

#include "CCI.h"
#include "Structures.h"
#include "Constants.h"
#include "UserPrimitives.h"
#include "TraceBuffer.h"
#include "KernelPointers.h"

#include <stdio.h>

MSG_queue * localQueue;
PCB* CCI_PCB;
void CCI()
{
	localQueue = MSG_queue_create();

	CCI_PCB = PID_to_PCB(CCI_PROCESS_PID);
	MsgEnv * msg;
	while (1)
	{
		
		
		msg = request_msg_env();
		msg->S_ID = CCI_PROCESS_PID;
		msg->type = CONSOLE_OUTPUT;
		strcat(msg->message, "\nCCI:");
		send_console_chars(msg);
		
		
		MsgEnv* displayAck;
		
       	while (1)
       	{
			displayAck = receive_message();
			if (displayAck->type != RETURN_ACK)
			{
				MSG_enqueue(localQueue, displayAck);
			}
			else {
				release_msg_env(displayAck);
				break;
			}
       	}
		
//repopulate the queue from the local queue
	while (!MSG_queue_isEmpty(localQueue)){
		MsgEnv* temp = MSG_dequeue(localQueue);
		MSG_enqueue(CCI_PCB->message_queue,temp);
	}
		msg = request_msg_env();
		msg->S_ID = CCI_PROCESS_PID;
		msg->type = KEYBOARD_INPUT;
		get_console_chars(msg);
		
		msg = receive_message();

		char str [MAXCHAR];
		strcpy(str, msg->message);

		switch (str[0])
		{
			case 's':
			case 'S':
				if (str[1] == '\0')
				{
					CCI_s(msg);
					break;
				}
				else
				{
					InvalidInput(msg);
					break;
				}
			case 'p':
			case 'P':
				if ( (str[1] == 's' || str[1] == 'S') && str[2] == '\0')
				{
					CCI_ps(msg);
					break;
					//break;
				}
				else
				{
					InvalidInput(msg);
					break;
				}

			case 'c':
			case 'C':
				if ( (str[1] == ' ') && (str[4] == ':') && (str[7] == ':') && (str[10] == '\0')) // && *(str+10) == "")
				{	
					CCI_c(str[2]-'0', str[3]-'0', str[5]-'0', str[6]-'0', str[8]-'0', str[9]-'0', msg);
					break;
				}				
				switch(str[1])
				{
					case 'd':
					case 'D':
						if (str[2] == '\0')
						{
							CCI_cd(msg);
							break;
						}
						else
						{	
							InvalidInput(msg);
							break;
						}
					case 't':
					case 'T':
						if (str[2] == '\0')
						{
							CCI_ct(msg);
							break;
						}
						else
						{
							InvalidInput(msg);
							break;
						}
					default:
						InvalidInput(msg);
						break;	
				}
				break;			
			case 'b':
			case 'B':
				if (str[1] == '\0')
				{
					CCI_b(msg);
					break;
				}
				else
				{
					InvalidInput(msg);
					break;
				}
			case 't':
			case 'T':
				if (str[1] == '\0')
				{
					CCI_t();
					break;
				}
				else
				{
					InvalidInput(msg);
					break;
				}
			case 'n':
			case 'N':
				if (str[1] == ' ' && str[3] == ' ' && str[5] == '\0')
				{
					CCI_n(msg, str[2]-'0', str[4]-'0');
					break;
				}
				else
				{
					InvalidInput(msg);
					break;
				}
			default:
				InvalidInput(msg);
				break;
		}
	}
}

void InvalidInput(MsgEnv * msg)
{
	msg->S_ID = CCI_PROCESS_PID;
	snprintf(msg->message, sizeof(msg->message), "Invalid Keyboard Input");
	send_console_chars(msg);

	MsgEnv* displayAck;
       	while (1)
       	{
			   displayAck = receive_message();
			   if (displayAck->type != RETURN_ACK)
			   {
				   MSG_enqueue(localQueue, displayAck);
			   }
			   else {
				   release_msg_env(displayAck);
				   break;
			   }
       	}

	//repopulate the queue from the local queue
	while (!MSG_queue_isEmpty(localQueue)){
		MsgEnv* temp = MSG_dequeue(localQueue);
		MSG_enqueue(CCI_PCB->message_queue,temp);
	}
}

void CCI_s(MsgEnv* msg)
{
	msg->S_ID = CCI_PROCESS_PID;
	send_message(USER_A, msg);
}

void CCI_ps(MsgEnv* msg)
{

	msg->S_ID = CCI_PROCESS_PID;
	request_process_status(msg);
	msg->type = CONSOLE_OUTPUT;
	send_console_chars(msg);


	MsgEnv* displayAck;
       	while (1)
       	{

			   displayAck = receive_message();
			   if (displayAck->type != RETURN_ACK)
			   {

				   MSG_enqueue(localQueue, displayAck);
			   }
			   else {
				   release_msg_env(displayAck);
				   break;
			   }
       	}

	//repopulate the queue from the local queue
	while (!MSG_queue_isEmpty(localQueue)){

		MsgEnv* temp = MSG_dequeue(localQueue);
		MSG_enqueue(CCI_PCB->message_queue,temp);
	}
}

void CCI_c(int h1, int h2, int m1, int m2, int s1, int s2, MsgEnv* msg)
{
	if (((h1 <= 2 && h2 <= 4) || (h1 <=1 &&h2 <= 9)) && m1 <= 5 && m2 <= 9 && s1 <=5 && s2 <=9)
	{
		WALL_CLOCK_HOURS_TIME = h1*10+h2;
		WALL_CLOCK_MINUTES_TIME = m1*10+m2;
		WALL_CLOCK_SECONDS_TIME = s1*10+s2;
		release_msg_env(msg);
	}
	else
	{
		MsgEnv* displayAck;
		while (1)
	       	{
				msg->S_ID = CCI_PROCESS_PID;
				msg->type = CONSOLE_OUTPUT;
				snprintf(msg->message, MAXCHAR, "Invalid Input");
				send_console_chars(msg);

				   displayAck = receive_message();
				   if (displayAck->type != RETURN_ACK)
				   {
					   MSG_enqueue(localQueue, displayAck);
				   }
				   else {
					   release_msg_env(displayAck);
					   break;
				   }
	       	}

		//repopulate the queue from the local queue
		while (!MSG_queue_isEmpty(localQueue)){
			MsgEnv* temp = MSG_dequeue(localQueue);
			MSG_enqueue(CCI_PCB->message_queue,temp);
		}	
	}
}

void CCI_cd(MsgEnv* msg)
{
	msg->S_ID = CCI_PROCESS_PID;
	snprintf(msg->message, MAXCHAR, "true");
	msg->type = CCI_DISPLAYCLOCK;
	msg->R_ID = CLOCK_PID;
	send_message(CLOCK_PID, msg);
}

void CCI_ct(MsgEnv* msg)
{
	msg->S_ID = CCI_PROCESS_PID;
	snprintf(msg->message, MAXCHAR, "false");
	msg->type = CCI_DISPLAYCLOCK;
	msg->R_ID = CLOCK_PID;
	send_message(CLOCK_PID, msg);
}

void CCI_b(MsgEnv* msgSEND)
{
	msgSEND->S_ID = CCI_PROCESS_PID;
	PrintTraceBufferSEND(msgSEND);
	msgSEND->type = CONSOLE_OUTPUT;
	send_console_chars(msgSEND);
	MsgEnv* displayAck;
       	while (1)
       	{
			   displayAck = receive_message();
			   if (displayAck->type != RETURN_ACK)
			   {
				   MSG_enqueue(localQueue, displayAck);
			   }
			   else {
				   release_msg_env(displayAck);
				   break;
			   }
       	}

	//repopulate the queue from the local queue
	while (!MSG_queue_isEmpty(localQueue)){
		MsgEnv* temp = MSG_dequeue(localQueue);
		MSG_enqueue(CCI_PCB->message_queue,temp);
	}
}

void CCI_t()
{
	terminate();
}

void CCI_n(MsgEnv* msg, int priority, int PID)
{
	msg->S_ID = CCI_PROCESS_PID;
	if (priority < 1 || priority > 3 || PID < 4 || PID > NUM_PROCESSES-1) //cant touch iProcess or Null Process
	{
		msg->type = CONSOLE_OUTPUT;
		snprintf(msg->message, MAXCHAR, "Invalid Priority or PID\n");
		send_console_chars(msg);
		
		MsgEnv* displayAck;		
		while (1)
       		{
			   displayAck = receive_message();
			   if (displayAck->type != RETURN_ACK)
			   {
				   MSG_enqueue(localQueue, displayAck);
			   }
			   else {
				   release_msg_env(displayAck);
				   break;
			   }
       		}

		//repopulate the queue from the local queue
		while (!MSG_queue_isEmpty(localQueue)){
			MsgEnv* temp = MSG_dequeue(localQueue);
			MSG_enqueue(CCI_PCB->message_queue,temp);
		}

	}
	else
	{	
		change_priority(priority, PID); // check casting to int
		release_msg_env(msg);
	}
}



