#ifndef KERNELPOINTERS_H
#define KERNELPOINTERS_H

//INCLUDE LIBs
#include <stdlib.h>

//INCLUDE HEADERS
#include "Constants.h"
#include "Structures.h"
#include "Initialize.h"
#include "RTX-G15.h"
#include "RPQ.h"

//DEFINE POINTERS

//PCB Queues - Arrays
PCB* PCBAddress[NUM_PROCESSES];
PCB* curr_proc1; // to look into this-> static, volatile?
RPQ_queue* RPQ;

//Blocked 
PCB_queue* Block_Env;

//MSG Queues
MSG_queue* Free_Msg_Queue;

//Shared Memory Pointers
inputbuf* in_mem_p;		// pointer to structure that is the shared memory for keyboard
inputbuf* out_mem_p;   		// pointer to structure that is the shared memory for CRT

//Init Table Pointer
initTable * initTable1;

int KERNEL_SECONDS_TIME; 
int KERNEL_MINUTES_TIME; 
int KERNEL_HOURS_TIME; 

int WALL_CLOCK_HOURS_TIME;
int WALL_CLOCK_MINUTES_TIME;
int WALL_CLOCK_SECONDS_TIME;

#endif
