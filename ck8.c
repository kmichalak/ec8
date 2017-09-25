#include <stdlib.h>
#include "cpu.h"
#include "ck8.h"
#include "timer.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>

char wait_for_key() {
	bool waits_for_key = true;

	SDL_Event event;
	char key_index;
	printf("Waiting for key \n");

	while (waits_for_key) {
		while (SDL_PollEvent(&event)) {
			uint8_t keyboard_state = SDL_GetKeyboardState(0);
			if (event.type == SDL_KEYDOWN) {
				SDL_Scancode keyIndex = event.key.keysym.scancode;

				switch (keyIndex) {
					case SDL_SCANCODE_0:
					case SDL_SCANCODE_1:
					case SDL_SCANCODE_2:
					case SDL_SCANCODE_3:
					case SDL_SCANCODE_4:
					case SDL_SCANCODE_5:
					case SDL_SCANCODE_6:
					case SDL_SCANCODE_7:
					case SDL_SCANCODE_8:
					case SDL_SCANCODE_9:
						printf("Got the key\n");
						key_index = SDL_SCANCODE_0 - keyIndex;
						waits_for_key = false;	
						break;
					default: 
						break;
				}
			
			}	
		}
	}

	return key_index;
}

static void run_cpu_cycle(Timer *timer, void *data) {
	printf("Handling tick\n");
	Cpu *cpu = (Cpu *) data;

	if (cpu->is_running(cpu)) {
		printf("CPU is running\n");
		cpu->fetch_opcode(cpu);
		cpu->handle_opcode(cpu);

		SDL_Event event;
		char key_index;

		while (SDL_PollEvent(&event)) {
			uint8_t keyboard_state = SDL_GetKeyboardState(0);
			if (event.type == SDL_KEYDOWN) {
				SDL_Scancode keyIndex = event.key.keysym.scancode;

				switch (keyIndex) {
					case SDL_SCANCODE_0:
					case SDL_SCANCODE_1:
					case SDL_SCANCODE_2:
					case SDL_SCANCODE_3:
					case SDL_SCANCODE_4:
					case SDL_SCANCODE_5:
					case SDL_SCANCODE_6:
					case SDL_SCANCODE_7:
					case SDL_SCANCODE_8:
					case SDL_SCANCODE_9:
						printf("Got the key\n");
						key_index = SDL_SCANCODE_0 - keyIndex;
						// waits_for_key = false;	
						break;
					case SDL_SCANCODE_Q:
						cpu->running = false;
						break;
					default: 
						break;
				}
				
			}	
		}
		timer->counter = 60;
	} else {
		printf("Shutting down CPU\n");
		timer->counter = 0;
		timer->enabled = false;
	}
}


int main(int argc, char* argv[]) {

	Cpu *cpu = malloc(sizeof(Cpu));
	initialize(cpu);

	Timer *main_timer = malloc(sizeof(Timer));
	init_timer(main_timer, cpu, run_cpu_cycle);

	shutdown_cpu(cpu);

	return 0;
};