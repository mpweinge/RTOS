#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include <stdlib.h>
#include "Structures.h"


/*FUNCTION DECLARATIONS
/*-------------------*/

int K_send(int target_pid, MsgEnv* message);

MsgEnv* K_receive();

MsgEnv* K_request_envelope();

int K_release_msg_env(MsgEnv* message);

int K_release_msg_env(MsgEnv* message);

int K_release_processor();

int K_request_process_status(MsgEnv* message);

int K_change_priority(int new_priority, int target_pid);

int K_request_delay(int time_delay, int wakeup_code, MsgEnv* message);

int K_send_console_chars(MsgEnv* message);

int K_get_console_chars(MsgEnv* message);

void K_terminate();

void context_switch(jmp_buf prev, jmp_buf next);

void process_switch();

void atomic(int onoff);

#endif
