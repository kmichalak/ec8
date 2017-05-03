#include <time.h>
#include <math.h>
#include <stdio.h>

#include "timer.h"

static void tick(Timer *timer) {
	unsigned short freq = 1000/60;

	struct timespec	spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	long time_diff = 
			round((spec.tv_nsec - timer->time.tv_nsec) / 1.0e6) 
			+ 1000 * (spec.tv_sec - timer->time.tv_sec); 


	if (timer->enabled && time_diff >= freq) {
		timer->time = spec;
		timer->counter -= 1;
		timer->enabled = timer->counter;
		timer->tick_callback(timer);
	}
}



void init_timer(Timer *timer, void (*tick_callback)(Timer *timer)) {
	timer->counter = 60;
	timer->enabled = true;
	timer->tick = tick;
	timer->tick_callback = tick_callback;
	clock_gettime(CLOCK_REALTIME, &(timer->time));

	while (timer->enabled) {
		timer->tick(timer);
	}
}