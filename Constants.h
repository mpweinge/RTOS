#ifndef CONSTANTS_H
#define CONSTANTS_H

//INCLUDE LIBs
#include <stdlib.h>


//DEFINE VARIABLES


//Define Kernel Constients
#define NUM_MESSAGES 120
#define NUM_PROCESSES 9
#define BUFFERSIZE 100
//#define MAXCHAR 800
#define STACKSIZE 16384
#define NULL_PRIORITY 4


//Define Process States
#define READY 1
#define EXECUTING 2
#define BLOCKED_RECIEVE 3
#define BLOCKED_ENV_REQUEST 4
#define INTERRUPTED 5
#define EXITING 6


//Define PIDs
#define KB_IPROCESS_PID 1
#define CRT_IPROCESS_PID 2
#define CLOCK_IPROCESS_PID 3
#define CLOCK_PID 4
#define USER_A 5
#define USER_B 6
#define USER_C 7
#define CCI_PROCESS_PID 8
#define NULL_PROCESS 9

//Message types
#define WAITING_CRT 2
#define KEYBOARD_INPUT 3
#define RETURN_ACK 4
#define CCI_DISPLAYCLOCK 5
#define COUNT_REPORT 6
#define MSG_TIME 7
#define CONSOLE_OUTPUT 8
//#define wakeup10 10

//Number of items to store in trace buffer
#define TRACE_BUFFER_NUM 16


//Constants for atomicity//
#define ON 1
#define OFF 0

//Buffer Size
#define BUFFERSIZE 100
#define MAXCHAR 2000

//ID's
#define I_PROCESS 0xFFFFFFFF

//Define Automic States
#define ON 1
#define OFF 0

//Define Return Codes
#define ERROR -1
#define SUCCESS 0

#endif
