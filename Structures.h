#ifndef STRUCTURES_H
#define STRUCTURES_H

//INCLUDE LIBs
#include <stdlib.h>
#include <setjmp.h>

//INCLUDE HEADERS
#include "Constants.h"


//---TYPE DEFINITIONS---

//Message Envelope Data Structure
typedef struct MSGnode
{
 	int R_ID;
	int S_ID;
	int type;
	struct MSGnode* next;
	char message[MAXCHAR];
}MsgEnv;

//Message Envelope Queue Structure
typedef struct 
{
 	MsgEnv* head;
	MsgEnv* tail;
	int size;
}MSG_queue;

//PCB Data Stucture
typedef struct PCBnode
{
 	int ID;
	int state;
	int priority;
	char* stack;
	jmp_buf* context;
	void (*main)(); // function handle for initialization jumping 
	MSG_queue* message_queue;
	struct PCBnode* next;
}PCB;

//PCB Queue Structure
typedef struct 
{
 	PCB* head;
	PCB* tail;
	int size;
}PCB_queue;



//---FUNCTION DECLARATIONS---

PCB_queue* PCB_queue_create();

MSG_queue* MSG_queue_create();

void PCB_queue_terminate(PCB_queue* Q);

void MSG_queue_terminate(MSG_queue* Q);

int PCB_enqueue(PCB_queue* Q, PCB* PCBpointer);

int MSG_enqueue(MSG_queue* Q, MsgEnv* MSGpointer);

PCB* PCB_dequeue(PCB_queue* Q);


//ADDED FUNCTION TO HEADER FILE
PCB* PCB_dequeue_PID(PCB_queue* Q, int PID);

MsgEnv* MSG_dequeue(MSG_queue* Q);

int PCB_queue_isEmpty(const PCB_queue* Q);

int MSG_queue_isEmpty(const MSG_queue* Q);

PCB* PID_to_PCB(const int PID);

#endif
