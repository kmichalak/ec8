#include <stdlib.h>
#include "cpu.h"
#include "ck8.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>



// opcode_handler ops_handlers[16];

// int emulate_cycle(Cpu *cpu) {
// 	cpu->fetch_opcode(cpu);
// 	cpu->handle_opcode(cpu);
// 	// update timers
// 	return 0;
// }

char wait_for_key() {
	bool waits_for_key = true;

	SDL_Event event;
	char key_index;
	printf("Waiting for key \n");

	while (waits_for_key) {
		while (SDL_PollEvent(&event)) {
			printf("Running SDL_PollEvent loop\n");
			uint8_t keyboard_state = SDL_GetKeyboardState(0);
			if (event.type = SDL_KEYDOWN) {
				printf("Key type is SDL_KEYDOWN\n");
				SDL_Scancode keyIndex = event.key.keysym.scancode;
				printf("Got scan code %d\n", keyIndex);
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
				// if (SDL_Scancode.SDL_SCANCODE_1 == keyIndex) {

				// }
			}	
		}
	}



	// while (!SDL_PollEvent(&event) && waits_for_key) {
		
	// 	printf("Running SDL_PollEvent loop\n");
	// 	uint8_t keyboard_state = SDL_GetKeyboardState(0);
	// 	if (event.type = SDL_KEYDOWN) {
	// 		printf("Key type is SDL_KEYDOWN\n");
	// 		SDL_Scancode keyIndex = event.key.keysym.scancode;
	// 		printf("Got scan code %d\n", keyIndex);
	// 		switch (keyIndex) {
	// 			case SDL_SCANCODE_0:
	// 			case SDL_SCANCODE_1:
	// 			case SDL_SCANCODE_2:
	// 			case SDL_SCANCODE_3:
	// 			case SDL_SCANCODE_4:
	// 			case SDL_SCANCODE_5:
	// 			case SDL_SCANCODE_6:
	// 			case SDL_SCANCODE_7:
	// 			case SDL_SCANCODE_8:
	// 			case SDL_SCANCODE_9:
	// 				key_index = SDL_SCANCODE_0 - keyIndex;
	// 				waits_for_key = false;	
	// 				break;
	// 			default: 
	// 				continue;
	// 		}
	// 		// if (SDL_Scancode.SDL_SCANCODE_1 == keyIndex) {

	// 		// }
	// 	}
	// }
	printf("Returning results\n");
	return key_index;
}

int main(int argc, char* argv[]) {

	Cpu *cpu = malloc(sizeof(Cpu));
	initialize(cpu);
	// SDL_Init(SDL_INIT_EVERYTHING);

	while (cpu->is_running(cpu)) {
		cpu->fetch_opcode(cpu);
		cpu->handle_opcode(cpu);

		SDL_Event event;
		char key_index;

		while (SDL_PollEvent(&event)) {
			printf("Running SDL_PollEvent loop\n");
			uint8_t keyboard_state = SDL_GetKeyboardState(0);
			if (event.type = SDL_KEYDOWN) {
				printf("Key type is SDL_KEYDOWN\n");
				SDL_Scancode keyIndex = event.key.keysym.scancode;
				printf("Got scan code %d\n", keyIndex);
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
				// if (SDL_Scancode.SDL_SCANCODE_1 == keyIndex) {

				// }
			}	
		}

	}

	// char key = wait_for_key();


	// SDL_Quit();
	// printf("-------------->");
	// printf(key);
	// printf("<--------------");


	// initialize(cpu);

	// while(1) {

		// run CPU cycle
		// draw graphics if draw flag is set
		// set keys
	// }

	shutdown_cpu(cpu);

	return 0;
};