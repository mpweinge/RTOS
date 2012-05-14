/*
 *  Interrupt.h
 *  
 *
 *  Created by Michael Weingert on 11-11-06.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <signal.h>
#include "Structures.h"

void interruptHand(int signal);
void ClockHandler();
void InitializeGlobals();
void InitializeDelayQueue();

typedef struct DelayNode
{
	int time;
	int wakeup;
	int delayedPCB_ID;
	MsgEnv* wakeupmsg;
	struct DelayNode* next;
	struct DelayNode* previous;
} DelayItem;

DelayItem * DelayQueueHead;
DelayItem * DelayQueueTail;

#endif
