/*
 *  CCI.h
 *  
 *
 *  Created by Michael Weingert on 11-11-14.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef CCI_H
#define CCI_H
#include "Constants.h"
#include "Structures.h"

char* input;

void CCI_s(MsgEnv* msg);
void CCI_ps(MsgEnv* msg);
void CCI_c();
void CCI_cd(MsgEnv* msg);
void CCI_ct(MsgEnv* msg);
void CCI_b(MsgEnv* msg);
void CCI_t();
void CCI_n(MsgEnv* msg, int priority, int PID);
void InvalidInput(MsgEnv* msg);
void CCI();

#endif
