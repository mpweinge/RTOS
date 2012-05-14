#ifndef PROCESSC_C
#define PROCESSC_C

#include "UserPrimitives.h"
#include "Structures.h"
#include "Constants.h"
#include "ProcessC.h"

#include <stdio.h>

void ProcessC()
{
	MSG_queue * localQueue = MSG_queue_create();
   	//MSG_queue_create(localQueue);
	
   	PCB * processC_PCB = PID_to_PCB(USER_C);
   	//initialization?	
   	while(1)
   	{
      		MsgEnv* env;
      		env = receive_message();
      		if(env->type==COUNT_REPORT)
      		{
         		if( atoi(env->message) % 20 ==0)
         		{
            			env->S_ID = USER_C;
            			snprintf(env->message, MAXCHAR, "Process C");
            			send_console_chars(env);
            			//wait for display acknowledge;
             		 	MsgEnv* wkupenv;
              			MsgEnv* displayAck;
              			while (1)
               			{
							displayAck = receive_message();
							if (displayAck->type != RETURN_ACK)
							{
								MSG_enqueue(localQueue, displayAck);
							}
							else 
							{
								//dont free displayAck, it will be used below
								break;	
							}
               			}

					displayAck->S_ID = USER_C;
					displayAck->R_ID = CLOCK_IPROCESS_PID;
					displayAck->type = COUNT_REPORT;
					displayAck->next = NULL;
					snprintf(displayAck->message, MAXCHAR, ""); 


               			request_delay(10, 10, displayAck); // waiting for tayler
               			while(1) 
               			{
							wkupenv = receive_message();

                     			if (wkupenv->type != 10)
                        			MSG_enqueue(localQueue, wkupenv);
                     			else
								{
									release_msg_env(wkupenv);
                         			break;
								}   
               			}
              
				//repopulate the queue from the local queue
						while (!MSG_queue_isEmpty(localQueue))
						{
							MsgEnv* temp = MSG_dequeue(localQueue);
							MSG_enqueue(processC_PCB->message_queue,temp);
						}
				}
				else
				{
					release_msg_env(env);
				}
			}
			else
			{
				release_msg_env(env);
			}	
			release_processor();
   	}
}
#endif
      
