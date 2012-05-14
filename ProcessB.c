#include "UserPrimitives.h"
#include "Structures.h"
#include "Constants.h"
#include "ProcessB.h"

#include <stdio.h>

void ProcessB(void)
{
	while(1)
	{
       		MsgEnv* env = receive_message();
			env->S_ID = USER_B;
       		env->R_ID = USER_C;
       		send_message(USER_C,env);
       		release_processor();
    	}
}

	
