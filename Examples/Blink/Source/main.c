#include "PJ_RPI_USER.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include "tlpi_hdr.h"

void sigalrm_handler(int signum)
{
	static volatile int count = 0;
	count++;
	if(count%2 == 0)
	{
		GPIO_SET = 1 << 17; // Set GPIO 17 high
	}
	else
	{
		GPIO_CLR = 1 << 17; // Set GPIO 17 low
	}
	// Do nothing, just interrupt sleep
}

int argv_demo(int a, ...)
{
	va_list args;
	va_start(args, a);
	for (int i = 0; i < a; i++) {
		int value = va_arg(args, int);
		printf("Argument %d: %d\n", i + 1, value);
	}
	va_end(args);
	return 0;
}

int init_timer(int count, ...)
{
	struct itimerval itv;
	// Initialize timer values
	itv.it_value.tv_sec = 3;
	itv.it_value.tv_usec = 0;
	itv.it_interval.tv_sec = 3;
	itv.it_interval.tv_usec = 0;

	va_list args;
	va_start(args, count);

	if(count > 0) {itv.it_value.tv_sec = va_arg(args, int);	}
	if(count > 1) {itv.it_value.tv_usec = va_arg(args, int);}
	if(count > 2) {itv.it_interval.tv_sec = va_arg(args, int);}
	if(count > 3) {itv.it_interval.tv_usec = va_arg(args, int);}

	va_end(args);

	if(setitimer(ITIMER_REAL, &itv, NULL) == -1) { 	errExit("setitimer");}
	return 0;
}

int main(int argc, char *argv[])
{

	argv_demo(3, 10, 20, 30);
	argv_demo(5, 1, 2, 3, 4, 5);

	struct itimerval itv;
	struct sigaction sa;

	if(map_peripheral(&gpio) == -1) 
	{
       	 	printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        	return -1;
    	}

	// Define gpio 17 as output
	INP_GPIO(17);
	OUT_GPIO(17);

	if(sigemptyset(&sa.sa_mask) == -1) {
		errExit("sigemptyset");
	}
	sa.sa_handler = sigalrm_handler;
	sa.sa_flags = 0;
	if(sigaction(SIGALRM, &sa, NULL) == -1) {
		errExit("sigaction");
	}


	init_timer(2, 5, 0); // na 2 seconden, 1 sec interval timer

	
	while(1)
	{
		// Toggle 17 (blink a led!)
		//GPIO_SET = 1 << 17;
		sleep(1);

		//GPIO_CLR = 1 << 17;
		sleep(1);
	}

	return 0;	

}
