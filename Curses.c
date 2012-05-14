/*
 *  Curses.c
 *  
 *
 *  Created by Michael Weingert on 11-11-23.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Curses.h"
#include <string.h>

int ymax;
int xmax;
int i=1;
int j=0;
double p;
char name[20];
char str[80];
char msg1[80]="Call trans opt: received. 24-11-11 12:19:05 REC:Log>";
char msg2[80]="Trace program: running";
char msg3[80]="Wake up, seviora...";
char msg4[80]="The Matrix has you...";
char msg5[80]="Follow the white rabbit.";
char msg6[80]="Knock, Knock, seviora.";
int q;	
int r;
int s;
int t;
int l;
int delay=15000;

void curses_init()
{


initscr();
start_color();
init_pair(1, COLOR_GREEN, COLOR_BLACK);
attron(COLOR_PAIR(1));	

getmaxyx(stdscr,ymax,xmax);
curs_set(0);

attron(A_BOLD);	
mvprintw(ymax/2,xmax/2-13,"INITIALIZING THE MATRIX..."); 			
refresh();

for(i;i<xmax-5;i++)
{	
	attron(A_ALTCHARSET);
	attron(A_BOLD);
	move(ymax/2+2,2+i);
	addch(ACS_CKBOARD);
	attroff(A_ALTCHARSET);
	usleep(4*delay);
	p=((double)i/(xmax-6))*100;
	mvprintw(ymax/2,xmax/2+13,"%.0f",p);
	mvprintw(ymax/2,xmax/2+16,"%c",37);
	attroff(A_BOLD);
	refresh();
}
i=0;

clear();
attron(A_BOLD);
curs_set(1);
refresh();

for(i;i<52;i++)
{
	mvprintw(0,i,"%c",msg1[i]);
	
	refresh();
	usleep(12*delay);
}
usleep(8000*delay);
clear();
refresh();
i=0;

for(i;i<22;i++)
{
	mvprintw(0,i,"%c",msg2[i]);
	refresh();
	usleep(12*delay);
}
usleep(8000*delay);
curs_set(0);
clear();
attroff(A_BOLD);
refresh();
i=0;

for(i;i<20;i++)
{
	s=rand()%xmax;
	t=rand()%xmax;
	
	for(j;j<ymax+25;j++)
	{
		r=rand()%10;
		q=rand()%10*2;
		
		attron(A_BOLD);		
		mvprintw(j,s,"%i",r);
		mvprintw(j-2*q,t,"%i",q);	
		refresh();
		attroff(A_BOLD);
		mvprintw(j-10,s,"%i",r);
		mvprintw(j-10,t,"%i",q);
		refresh();
		
		usleep(delay/2);
	}
	j=0;
}
attroff(A_ALTCHARSET);
clear();
refresh();

i=0;

attron(A_BOLD);
curs_set(1);

for(i;i<19;i++)
{
	mvprintw(0,i,"%c",msg3[i]);
	
	refresh();
	usleep(90*delay);
}
i=0;

usleep(8000*delay);
clear();
refresh();
i=0;

for(i;i<21;i++)
{
	mvprintw(0,i,"%c",msg4[i]);
	
	refresh();
	usleep(90*delay);
}
usleep(16000000*delay);
clear();
refresh();
i=0;

for(i;i<24;i++)
{
	mvprintw(0,i,"%c",msg5[i]);
	
	refresh();
	usleep(30*delay);
}
usleep(16000*delay);
clear();
refresh();
i=0;

for(i;i<22;i++)
{
	mvprintw(0,i,"%c",msg6[i]);
	
	refresh();
	usleep(90*delay);
}
i=0;

usleep(16000*delay);
clear();
refresh();
i=0;

attroff(A_BOLD);
clear();
refresh();

curs_set(0);
clear();

/*for(i;i<20;i++)
{
	s=rand()%xmax;
	t=rand()%xmax;
	
	for(j;j<ymax+25;j++)
	{
		r=rand()%10;
		q=rand()%10*2;
		
		attron(A_BOLD);		
		mvprintw(j,s,"%i",r);
		mvprintw(j-2*q,t,"%i",q);	
		refresh();
		attroff(A_BOLD);
		mvprintw(j-10,s,"%i",r);
		mvprintw(j-10,t,"%i",q);
		refresh();
		
		usleep(delay);
	}
	j=0;
}*/
attroff(A_ALTCHARSET);
clear();
curs_set(1);
refresh();
endwin();
}

void curses_terminate()
{
	
	initscr();
	clear();
	refresh();
	curs_set(0);
	i=0;
	for(i;i<12;i++)
	{
		s=rand()%xmax;
		t=rand()%xmax;
		
		for(j;j<ymax+25;j++)
		{
			r=rand()%10;
			q=rand()%10*2;
			
			attron(A_BOLD);		
			mvprintw(j,s,"%i",r);
			mvprintw(j-2*q,t,"%i",q);	
			refresh();
			attroff(A_BOLD);
			mvprintw(j-10,s,"%i",r);
			mvprintw(j-10,t,"%i",q);
			refresh();
			
			usleep(delay/2);
		}
		j=0;
	}
	
	attroff(A_ALTCHARSET);
	attron(A_BOLD);
	mvprintw(ymax/2-1,xmax/2-12,"                       ");
	mvprintw(ymax/2+1,xmax/2-12,"                       ");
	mvprintw(ymax/2,xmax/2-11," TRACE PROGRAM STOPPED ");
	refresh();
	attroff(A_BOLD);
	usleep(700*delay);
	clear();
	refresh();
	endwin();
}

