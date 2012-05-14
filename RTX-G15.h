//INCLUDE LIBs
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <setjmp.h>

//INCLUDE HEADERS
#include "Constants.h"

#ifndef MAIN_H
#define MAIN_H

typedef struct {
	int ok_flag;
	char indata[MAXCHAR];
	int length;
} inputbuf;

//Jump Buffer for SetJmp, LongJmp Command for Kernel Initilization
jmp_buf kernel_buf;

int kbfid;
int crtfid;

int kb_pid; 
int crt_pid;

//typedef char* caddr_t;

#endif
