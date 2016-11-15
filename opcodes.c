#include <stdlib.h>
#include "opcodes.h"

// 00EX
void handle_0(Cpu *cpu) {
	unsigned short opcode_arg = cpu->opcode & 0x0fff;
	switch (opcode_arg) {
		case 0x0E0:
			// clean screen
			break;
		case 0x0EE:
			cpu->sp -= 1;
			cpu->PC = cpu->stack[cpu->sp];
			break;
		default:
			// Call program at address NNN
			break;
	}
	cpu->PC += 2;
}

// 00E0
void clear_screen(Cpu *cpu) {}

// 00EE
void return_from_rsubutine(Cpu *cpu) {}

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
void vx_equals_vy(Cpu *cpu) {}

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
}

// DXYN
void draw(Cpu *cpu) {}

// EXNN
void handle_key(Cpu *cpu) {}

// FXNN
void handle_f(Cpu *cpu) {}

