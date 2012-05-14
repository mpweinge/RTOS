/*
 *  NullProcess.c
 *  
 *
 *  Created by Michael Weingert on 11-11-18.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "UserPrimitives.h"

void NullProcess()
{
	while (1)
		release_processor();
}

