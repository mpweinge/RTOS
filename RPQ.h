#ifndef RPQ_H
#define RPQ_H

//INCLUDE LIBs
#include <stdlib.h>

//INCLUDE HEADERS
#include "Structures.h"

//---TYPE DEFINITIONS---

typedef struct 
{
 	PCB_queue* P0;
	PCB_queue* P1;
	PCB_queue* P2;
	PCB_queue* P3;
	PCB* NP;
}RPQ_queue;


//---FUNCTION DECLARATIONS---

RPQ_queue* RPQ_create();

void RPQ_terminate(RPQ_queue* Q);

int RPQ_enqueue(RPQ_queue* Q, PCB* PCBpointer);

PCB* RPQ_dequeue(RPQ_queue* Q);

PCB* RPQ_dequeue_PID(RPQ_queue* Q, int PID);

#endif
