/*
 *  CRT.c
 *  
 *
 *  Created by Michael Weingert on 11-11-09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CRT.h"

char * crtFileName = "CRT_Shared_Memory";

void terminate(int signal)
{
	exit(0);
}

//note that argc is the name of the process and argv are the arguments passed in
int main (int argc, char * argv[])
{	
	int parent_pid, fid;
	
	caddr_t mmap_ptr;
	
	inputbuf * out_mem_p;
	
	
	//if parent tells us to terminate, we should do so probably lolz
	sigset(SIGINT, terminate);
	
	
	sscanf(argv[1], "%d", &parent_pid);
	sscanf(argv[2], "%d", &fid);
	
	//map the shared memory
	mmap_ptr = mmap((caddr_t) 0,   /* Memory Location, 0 lets O/S choose */
					bufsize,/* How many bytes to mmap */
					PROT_READ | PROT_WRITE, /* Read and write permissions */
					MAP_SHARED,    /* Accessible by another process */
					fid,           /* which file is associated with mmap */
					(off_t) 0);    /* Offset in page frame */
    if (mmap_ptr == MAP_FAILED){
		printf("Child memory map has failed, CRT is aborting!\n");
		terminate(0);
    }
	
	out_mem_p = (inputbuf *)mmap_ptr;
	
	//check flag in memory
	while (1)
	{
		if (out_mem_p->ok_flag == 0)
		{
			usleep(100000);
			continue;
		}
		else {
			printf("%s", out_mem_p->indata);
			fflush(stdout);
			out_mem_p->ok_flag = 0;
			//signal parent
			kill(parent_pid,SIGUSR2);
		}
		
	}
}
