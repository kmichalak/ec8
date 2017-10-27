#include <stdlib.h>
#include "cpu.h"
#include "ck8.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>



static void run_cpu_cycle(Timer *timer, void *data) {
	Cpu *cpu = (Cpu *) data;

	if (cpu->is_running(cpu)) {
		
		SDL_Event event;
		uint8_t key_index;

		while (SDL_PollEvent(&event)) {
			uint8_t keyboard_state = SDL_GetKeyboardState(0);
			if (event.type == SDL_KEYDOWN) {
				SDL_Scancode key_scancode = event.key.keysym.scancode;

				switch (key_scancode) {
					case SDL_SCANCODE_0:
						cpu->key[0] = 1;
						break;
					case SDL_SCANCODE_1:
						cpu->key[1] = 1;
						break;
					case SDL_SCANCODE_2:
						cpu->key[2] = 1;
						break;
					case SDL_SCANCODE_3:
						cpu->key[3] = 1;
						break;
					case SDL_SCANCODE_4:
						cpu->key[4] = 1;
						break;
					case SDL_SCANCODE_5:
						cpu->key[5] = 1;
						break;
					case SDL_SCANCODE_6:
						cpu->key[6] = 1;
						break;
					case SDL_SCANCODE_7:
						cpu->key[7] = 1;
						break;
					case SDL_SCANCODE_8:
						cpu->key[8] = 1;
						break;
					case SDL_SCANCODE_9:
						cpu->key[9] = 1;
						break;
					case SDL_SCANCODE_A:
						cpu->key[0xA] = 1;
						break;
					case SDL_SCANCODE_B:
						cpu->key[0xB] = 1;
						break;
					case SDL_SCANCODE_C:
						cpu->key[0xC] = 1;
						break;
					case SDL_SCANCODE_D:
						cpu->key[0xD] = 1;
						break;
					case SDL_SCANCODE_E:
						cpu->key[0xE] = 1;
						break;
					case SDL_SCANCODE_F:
						cpu->key[0xF] = 1;
						break;
					case SDL_SCANCODE_Q:
						cpu->running = false;
						break;
					default: 
						break;
				}


			}	
			else if (event.type == SDL_KEYUP) {
				SDL_Scancode key_scancode = event.key.keysym.scancode;

				switch (key_scancode) {
					case SDL_SCANCODE_0:
						cpu->key[0] = 0;
						break;
					case SDL_SCANCODE_1:
						cpu->key[1] = 0;
						break;
					case SDL_SCANCODE_2:
						cpu->key[2] = 0;
						break;
					case SDL_SCANCODE_3:
						cpu->key[3] = 0;
						break;
					case SDL_SCANCODE_4:
						cpu->key[4] = 0;
						break;
					case SDL_SCANCODE_5:
						cpu->key[5] = 0;
						break;
					case SDL_SCANCODE_6:
						cpu->key[6] = 0;
						break;
					case SDL_SCANCODE_7:
						cpu->key[7] = 0;
						break;
					case SDL_SCANCODE_8:
						cpu->key[8] = 0;
						break;
					case SDL_SCANCODE_9:
						cpu->key[9] = 0;
						break;
					case SDL_SCANCODE_A:
						cpu->key[0xA] = 0;
						break;
					case SDL_SCANCODE_B:
						cpu->key[0xB] = 0;
						break;
					case SDL_SCANCODE_C:
						cpu->key[0xC] = 0;
						break;
					case SDL_SCANCODE_D:
						cpu->key[0xD] = 0;
						break;
					case SDL_SCANCODE_E:
						cpu->key[0xE] = 0;
						break;
					case SDL_SCANCODE_F:
						cpu->key[0xF] = 0;
						break;
					case SDL_SCANCODE_Q:
						cpu->running = false;
						break;
					default: 
						break;
				}

			}
		}

		cpu->fetch_opcode(cpu);
		cpu->handle_opcode(cpu);
		if (cpu->dt > 0) {		
			cpu->dt = cpu->dt - 1;
		}
		if (cpu->st > 0) {
			cpu->st = cpu->st - 1;
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