/*
 *  TraceBuffer.h
 *  
 *
 *  Created by Michael Weingert on 11-11-14.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TRACEBUFFER_H
#define TRACEBUFFER_H

typedef struct TraceNode
{
	int S_ID;
	int R_ID;
	int TimeStampSec;
	int TimeStampMin;
	int TimeStampHr;
	int Message_Type;
	struct TraceNode* next;
	struct TraceNode* previous;
}TraceBuffer;

TraceBuffer* headSEND;
TraceBuffer* tailSEND;

TraceBuffer* headRECEIVE;
TraceBuffer* tailRECEIVE;

#endif
