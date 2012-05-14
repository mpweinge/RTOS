RTX-G15OBJS = CCI.o Clean.o CLOCK.o Initialize.o Interrupt.o RTX-G15.o Primitives.o ProcessA.o ProcessB.o ProcessC.o RPQ.o Structures.o TraceBuffer.o NullProcess.o Curses.o UserPrimitives.o
KBOBJS = Keyboard.o
CRTOBJS = CRT.o

all: RTX-G15 Keyboard CRT

CC = gcc

RTX-G15: $(RTX-G15OBJS)
	$(CC) -o RTX-G15 $(RTX-G15OBJS) -lncurses

Keyboard: $(KBOBJS)
	$(CC) -o Keyboard $(KBOBJS)

CRT: $(CRTOBJS)
	$(CC) -o CRT $(CRTOBJS)

CCI.o : CCI.c
	$(CC) -c CCI.c

Clean.o : Clean.c
	$(CC) -c Clean.c

CLOCK.o : CLOCK.c
	$(CC) -c CLOCK.c

Curses.o : Curses.c
	$(CC) -c Curses.c

Initialize.o : Initialize.c
	$(CC) -c Initialize.c

Interrupt.o : Interrupt.c 
	$(CC) -c Interrupt.c 

RTX-G15.o : RTX-G15.c
	$(CC) -c RTX-G15.c

NullProcess.o : NullProcess.c
	$(CC) -c NullProcess.c

Primitives.o : Primitives.c
	$(CC) -c Primitives.c

ProcessA.o : ProcessA.c
	$(CC) -c ProcessA.c

ProcessB.o : ProcessB.c
	$(CC) -c ProcessB.c

ProcessC.o : ProcessC.c
	$(CC) -c ProcessC.c

RPQ.o : RPQ.c
	$(CC) -c RPQ.c

Structures.o : Structures.c
	$(CC) -c Structures.c

TraceBuffer.o : TraceBuffer.c
	$(CC) -c TraceBuffer.c

UserPrimitives.o : UserPrimitives.c
	$(CC) -c UserPrimitives.c

Keyboard.o : Keyboard.c
	$(CC) -c Keyboard.c

CRT.o : CRT.c
	$(CC) -c CRT.c

clean :
	rm *.o
	rm Keyboard_Shared_Memory
	rm CRT_Shared_Memory
