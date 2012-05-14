/*
 *  CLOCK.c
 *  
 *
 *  Created by Michael Weingert on 11-11-14.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CLOCK.h"
#include "Constants.h"
#include "Structures.h"
#include "UserPrimitives.h"
#include "KernelPointers.h"

#include <stdlib.h>
#include <stdio.h>

int display;
PCB* CLOCK;

void ClockMain()
{
	CLOCK = PID_to_PCB(CLOCK_PID);
	MsgEnv* temp;
	MSG_queue * localQueue = MSG_queue_create();
	
	display = 0;

	MsgEnv* msg;
	
	while (1)
	{

			if (display)
			{			
				MsgEnv* newmsg = request_msg_env();
				newmsg->S_ID = CLOCK_PID;
				newmsg->type = MSG_TIME;
				newmsg->R_ID = CLOCK_IPROCESS_PID;
				send_message(CLOCK_IPROCESS_PID, newmsg); //will get return with time			
			}			

			msg = receive_message();
			if (msg->type != CCI_DISPLAYCLOCK) // will enter here when iprocess sends current time
			{
				if (display)
				{
					msg->S_ID = CLOCK_PID;
					msg->type = CONSOLE_OUTPUT;
					msg->R_ID = CRT_IPROCESS_PID;
					send_console_chars(msg);
					while (1)
					{
						msg = receive_message();
						if (msg->type == RETURN_ACK)
						{
							msg->S_ID = CLOCK_PID;
							release_msg_env(msg);	
							break;
						}
						else 
						{
							MSG_enqueue(localQueue, msg);
						}
					}
					
					while ( (MSG_queue_isEmpty(localQueue)) == 0)
					{
							MsgEnv* temp = MSG_dequeue(localQueue);
							MSG_enqueue(CLOCK->message_queue,temp);
					}
				}
				else
				{
					release_msg_env(msg);	
				}
			}			
			else //will enter here when user types to CD or CT aka change whether clock is displaying
			{
				if (!(strcmp(msg->message,"true")))
				{
					display = 1;
				}
				else
				{ // (msg->message == "false");
					display = 0;
				}

				release_msg_env(msg);
			}
			release_processor();
	}
}



