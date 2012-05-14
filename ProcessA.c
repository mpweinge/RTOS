#include "UserPrimitives.h"
#include "Structures.h"
#include "Constants.h"
#include "ProcessA.h"

#include <stdio.h>

void ProcessA(void)
{
	MsgEnv* env = receive_message();
	//perhaps check message to make sure it is right one
	//shouldn't be an issue, however
	release_msg_env(env);
	int num = 1;
	while(1)
	{
        	MsgEnv* env = request_msg_env();
        	env->type = COUNT_REPORT;
		snprintf(env->message, MAXCHAR, "%i", num);
        	env->S_ID = USER_A;
        	env->R_ID = USER_B;
        	send_message(USER_B,env);
        	num++;
     	  	release_processor();
   	 }	
}
