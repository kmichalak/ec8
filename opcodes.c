#include <stdlib.h>
#include <string.h> // just for memcpy and memset

#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>
#include "opcodes.h"

#include <stdio.h>

// 00EX
void handle_0(Cpu *cpu) {
	unsigned short opcode_arg = cpu->opcode & 0x0fff;
	switch (opcode_arg) {
		case 0x0E0:
			clear_screen(cpu);
			cpu->PC += 2;
			break;
		case 0x0EE:
			return_from_rsubutine(cpu);
			break;
		default:
			// Call program at address NNN
			// cpu->PC = cpu->opcode & 0x0fff;
			cpu->PC += 2;
			// cpu->stack[cpu->sp] = cpu->PC & 0xffff;
			// cpu->sp += 1;

			// // cpu->PC = cpu->opcode & 0x0fff;
			// cpu->PC = opcode_arg;

			// cpu->fetch_opcode(cpu);
			// cpu->handle_opcode(cpu);
			break;
	}
}

// 00E0
void clear_screen(Cpu *cpu) {
	cpu->display->clear_screen(cpu->display);
	// memset(cpu->display->screen, 0, sizeof(cpu->display->screen));
}

// 00EE
void return_from_rsubutine(Cpu *cpu) {
	cpu->PC = cpu->stack[cpu->sp];
	cpu->sp -= 1;
}

// 1NNN
void jump(Cpu *cpu) {
	cpu->PC = cpu->opcode & 0x0fff;
}

// 2NNN
void call_subrutine(Cpu *cpu) {
	cpu->stack[cpu->sp] = cpu->PC & 0xffff;
	cpu->sp += 1;

	cpu->PC = cpu->opcode & 0x0fff;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);
}

// 3XNN
void vx_equals(Cpu *cpu) {
	unsigned short reg_num = (cpu->opcode & 0x0f00) >> 8;
	unsigned char val = cpu->opcode & 0x00ff;
	if (cpu->registers[reg_num] == val) {
		cpu->PC += 2;
	}
	cpu->PC += 2;
}

// 4XNN
void vx_not_equals(Cpu *cpu) {
	unsigned short reg_num = (cpu->opcode & 0x0f00) >> 8;
	unsigned char val = cpu->opcode & 0x00ff;
	if (cpu->registers[reg_num] != val) {
		cpu->PC += 2;
	}
	cpu->PC += 2;
}

// 5XY0
void vx_equals_vy(Cpu *cpu) {
	unsigned short x_reg_num = (cpu->opcode & 0x0f00) >> 8;
	unsigned short y_reg_num = (cpu->opcode & 0x00f0) >> 4;

	if (cpu->registers[x_reg_num] == cpu->registers[y_reg_num]) {
		cpu->PC += 2;
	}
	cpu->PC += 2;
}

// 6XNN
void set_vx(Cpu *cpu) {
	unsigned short reg_num = (cpu->opcode & 0x0f00) >> 8;
	cpu->registers[reg_num] = cpu->opcode & 0x00ff;
	cpu->PC += 2;
}

// 7XNN
void add_to_vx(Cpu *cpu) {
	unsigned short reg_num = (cpu->opcode & 0x0f00) >> 8;
	cpu->registers[reg_num] += cpu->opcode & 0x00ff;
	cpu->PC += 2;
}

// 8XYN
void handle_8(Cpu *cpu) {
	unsigned short op_type = cpu->opcode & 0x000f;
	unsigned short x_reg = (cpu->opcode & 0x0f00) >> 8;
	unsigned short y_reg = (cpu->opcode & 0x00f0) >> 4;

	switch (op_type) {
		case 0: {
			cpu->registers[x_reg] = cpu->registers[y_reg];
			break;
		}
		case 1: {
			unsigned char val = cpu->registers[x_reg] | cpu->registers[y_reg];
			cpu->registers[x_reg] = val;
			break;
		}
		case 2: {
			unsigned char val = cpu->registers[x_reg] & cpu->registers[y_reg];
			cpu->registers[x_reg] = val;
			break;
		}
		case 3: {
			unsigned char val = cpu->registers[x_reg] ^ cpu->registers[y_reg];
			cpu->registers[x_reg] = val;
			break;
		}
		case 4: {
			unsigned short val = cpu->registers[x_reg] + cpu->registers[y_reg];
			cpu->registers[x_reg] = val & 0xff;
			cpu->registers[0xf] = (val & 0xff00) > 0;
			break;
		}
		case 5: {
			cpu->registers[0xf] = cpu->registers[x_reg] > cpu->registers[y_reg];
			short val = cpu->registers[x_reg] - cpu->registers[y_reg];
			cpu->registers[x_reg] = val & 0xff;
			break;
		}
		case 6: {
			unsigned short lsb = cpu->registers[x_reg] & 0x1;
			cpu->registers[x_reg] = cpu->registers[x_reg] >> 1;
			cpu->registers[0xf] = lsb;
			break;
		}
		case 7: {
			cpu->registers[0xf] = cpu->registers[x_reg] < cpu->registers[y_reg];
			short val = cpu->registers[y_reg] - cpu->registers[x_reg];
			cpu->registers[x_reg] = val;
			break;
		}
		case 0xe: {
			unsigned short msb = (cpu->registers[x_reg] & 0x80) >> 7;
			cpu->registers[x_reg] = cpu->registers[x_reg] << 1;
			cpu->registers[0xf] = msb;
			break;
		}
		default:
			break;
	}
}

// 9XY0
void vx_not_equals_vy(Cpu *cpu) {
	unsigned short x_reg_num = (cpu->opcode & 0x0f00) >> 8;
	unsigned short y_reg_num = (cpu->opcode & 0x00f0) >> 4;

	// Last part of opcode should be checked as if the value is different
	// than 0 the opcode is an unsupported one.

	if (cpu->registers[x_reg_num] != cpu->registers[y_reg_num]) {
		cpu->PC += 2;
	}
	cpu->PC += 2;
}

// ANNN
void set_prog_counter(Cpu *cpu) {
	cpu->I = cpu->opcode & 0x0fff;
	cpu->PC += 2;
}

// BNNN
void offset_jump(Cpu *cpu) {
	cpu->PC = (cpu->opcode & 0x0fff) + cpu->registers[0];
}

// CXNN
void random_vx(Cpu *cpu) {
	unsigned short val = cpu->opcode & 0x00ff;
	unsigned short reg_num = (cpu->opcode & 0x0f00) >> 8;

	cpu->registers[reg_num] = val & (rand() % 256);
	cpu->PC += 2;
}

const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

// DXYN
void draw(Cpu *cpu) {

	unsigned short xpos_reg = (cpu->opcode & 0x0f00) >> 8;
	unsigned short ypos_reg = (cpu->opcode & 0x00f0) >> 4;
	unsigned short bytes_num = (cpu->opcode & 0x000f);

	unsigned char xpos = cpu->registers[xpos_reg];
	unsigned char ypos = cpu->registers[ypos_reg];

	unsigned char sprite[bytes_num];

	for (int i=0; i< bytes_num; i++) {
		sprite[i] = cpu->memory[cpu->I + i];
	}

	// SDL_SetRenderDrawColor(cpu->display->renderer, 0, 0, 0, 0);		
			
	cpu->display->put_pixels(cpu->display, sprite, bytes_num, xpos, ypos);
	cpu->display->write_collision_state(cpu->display, &cpu->registers[0xf]);

	cpu->PC += 2;
}

// EXNN
void handle_key(Cpu *cpu) {
	unsigned short reg_num = (cpu->opcode & 0x0f00) >> 8;
	unsigned short operation = (cpu->opcode & 0x00ff);

	SDL_Event event;
	char current_key = -1; 

	while (SDL_PollEvent(&event)) {
		// uint8_t keyboard_state = SDL_GetKeyboardState(0);
		
		if (event.type == SDL_KEYDOWN) {
			SDL_Scancode key_index = event.key.keysym.scancode;
			switch (key_index) {
				case SDL_SCANCODE_0:
					current_key = 0;
					break;
				case SDL_SCANCODE_1:
					current_key = 1;
					break;
				case SDL_SCANCODE_2:
					current_key = 2;
					break;
				case SDL_SCANCODE_3:
					current_key = 3;
					break;
				case SDL_SCANCODE_4:
					current_key = 4;
					break;
				case SDL_SCANCODE_5:
					current_key = 5;
					break;
				case SDL_SCANCODE_6:
					current_key = 6;
					break;
				case SDL_SCANCODE_7:
					current_key = 7;
					break;
				case SDL_SCANCODE_8:
					current_key = 8;
					break;
				case SDL_SCANCODE_9:
					current_key = 9;
					break;
				default:
					break;
			}
		}
	}

	if (operation == 0x9E) {
		// SKP Vx - skip next instruction if key with the value of Vx is pressed
		if (cpu->registers[reg_num] == current_key) {
			cpu->PC += 1;
		}
	} else if (operation == 0xA1) {
		// SKNP Vx - skip next intruction if the key with value of Vx is not 
		// 			 pressed
		if (cpu->registers[reg_num] != current_key) {
			cpu->PC += 2;
		}
	}
	cpu->PC += 2;

	// SDL_Event event;
	// while (SDL_PollEvent(&event)) {

	// 	uint8_t keyboard_state = SDL_GetKeyboardState(0);

	// 	if (event.type == SDL_KEYDOWN) {
	// 		SDL_Scancode keyIndex = event.key.keysym.scancode;
	// 		// if (SDL_Scancode.SDL_SCANCODE_1 == keyIndex) {
	// 			printf("Got the key \n");
	// 		// }
	// 	}

	// }
}

// FXNN
void handle_f(Cpu *cpu) {

	unsigned short reg_num = (cpu->opcode & 0x0f00) >> 8;
	unsigned short operation = (cpu->opcode & 0x00ff);
	unsigned char value = cpu->registers[reg_num];

	switch (operation) {
		case 0x07:
			cpu->registers[reg_num] = cpu->dt;
			break;
		case 0x0A:
			// Wait for a key press, store the value of the key in Vx.
			// All execution stops until a key is pressed, then value of that
			// key is stored in Vx. 
			// TODO: What about timers, should they also be stopped, or they 
			// 		 are just running?
			// cpu->registers[reg_num] = wait_for_key();
			break;
		case 0x15:
			cpu->dt = value;
			break;
		case 0x18:
			cpu->st = value;
			break;
		case 0x1E:
			cpu->I = cpu->I + value;
			break;
		case 0x29:
			break;
		case 0x33:
			for (int i=2; i >= 0; i--) {
				cpu->memory[cpu->I + i] = value % 10;
				value /= 10;
			}
			break;
		case 0x55:
			memcpy(cpu->memory + cpu->I, cpu->registers, 
				   (reg_num + 1) * sizeof(unsigned char));
			break;
		case 0x65:
			memcpy(cpu->registers, cpu->memory + cpu->I, 
				   (reg_num + 1) * sizeof(unsigned char));
			break;
	}


}
