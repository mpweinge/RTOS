#include "UserPrimitives.h"
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

int send_message(int target_pid, MsgEnv* message)
{
   atomic(ON);
   int return_code = K_send(target_pid, message);
   atomic(OFF);
   return return_code;
}

MsgEnv* receive_message()
{
   atomic(ON);
   MsgEnv* tempMSG = K_receive();
	atomic(OFF);
	return tempMSG;
}

MsgEnv* request_msg_env()
{
    atomic(ON);
 	MsgEnv* tempMSG = K_request_envelope();
   atomic(OFF);
 	return tempMSG;
}

int release_msg_env(MsgEnv* message)
{
 atomic(ON);
   int return_code = K_release_msg_env(message);
   atomic(OFF);
	return return_code;
}

int release_processor()
{
   atomic(ON);
   int return_code = K_release_processor();
   atomic(OFF);
	return return_code;
}

int request_process_status(MsgEnv* message)
{
   atomic(ON);
   int return_code = K_request_process_status(message);
   atomic(OFF);	
	return return_code;
}

int change_priority(int new_priority, int target_pid)
{
   atomic(ON);
   int return_code = K_change_priority(new_priority, target_pid);
   atomic(OFF);
	return return_code;
}

int request_delay(int time_delay, int wakeup_code, MsgEnv* message)
{
   atomic(ON);
   int return_code = K_request_delay(time_delay, wakeup_code, message);
   atomic(OFF);   return return_code;
}

int send_console_chars(MsgEnv* message)
{
   atomic(ON);
   int return_code = K_send_console_chars(message);
   atomic(OFF);
	return return_code;
}

int get_console_chars(MsgEnv* message)
{
	atomic(ON);
   int return_code = K_get_console_chars(message);
   atomic(OFF);
	return return_code;
}

void terminate()
{
   K_terminate();
}
