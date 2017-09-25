#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cmocka.h>
#include <time.h>

#include <math.h>

#include "../timer.h"



static void handle_tick(Timer *timer, void *data) {
	printf("Tick handled: %d\n", timer->counter);
}

static void test_timer(void **state) {
	Timer *timer = malloc(sizeof(Timer));

	struct timespec start;
	struct timespec stop;

	clock_gettime(CLOCK_REALTIME, &start);

	init_timer(timer, handle_tick);

	clock_gettime(CLOCK_REALTIME, &stop);

	long time_diff = 
		round((stop.tv_nsec - start.tv_nsec) / 1.0e6)
		+ 1000 * (stop.tv_sec - start.tv_sec);

	// assume that we can have about 50 milliseconds of variation on the timer 
	assert_true(time_diff >= 950);
	assert_true(time_diff <= 1000);
}

int main(int argc, char **argv) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_timer)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);

}