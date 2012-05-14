/*
 *  CRT.h
 *  
 *
 *  Created by Michael Weingert on 11-11-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <signal.h>

#include <fcntl.h>

#include "RTX-G15.h" //replace with constants.h

#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

int bufsize = BUFFERSIZE;
