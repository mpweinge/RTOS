#ifndef INITIALIZE_H
#define INITIALIZE_H

//INCLUDE LIBs
#include <stdlib.h>

//INCLUDE HEADERS
#include "Constants.h"

//typedef void (*func)();

typedef struct {
	int ID;
	int state;
	int priority;
	int StackSize;
	void (*main)(); // function handle for jumping
} initTable;

int CreateInitializationTable();

int Initialize_MSG_Queues();

int Initialize_PCB_Queues();

int Initialize_PCBs();


#endif
