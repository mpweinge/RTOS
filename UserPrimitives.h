#ifndef USERPRIMITIVES_H
#define USERPRIMITIVES_H
#include <stdlib.h>
#include "Structures.h"


/*FUNCTION DECLARATIONS
/*-------------------*/

int send_message(int target_pid, MsgEnv* message);

MsgEnv* receive_message();

MsgEnv* request_msg_env();

int release_msg_env(MsgEnv* message);

int release_processor();

int request_process_status(MsgEnv* message);

int change_priority(int new_priority, int target_pid);

int request_delay(int time_delay, int wakeup_code, MsgEnv* message);

int send_console_chars(MsgEnv* message);

int get_console_chars(MsgEnv* message);

void terminate();

#endif
