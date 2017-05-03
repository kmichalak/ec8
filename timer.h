#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdbool.h>

#define FREQUENCY 60;	// 60Hz timer

typedef struct Timer Timer;

void init_timer(Timer *timer, void (*tick_calback)(Timer *timer));

struct Timer {
	unsigned short counter;
	struct timespec time;
	bool enabled;
	void (*tick)(Timer *timer);
	void (*tick_callback)(Timer *timer);
};

#endif