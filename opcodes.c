#include "opcodes.h"

// 00EX
void handle_0(Cpu *cpu) {}

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
void vx_not_equals(Cpu *cpu) {}

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
void handle_8(Cpu *cpu) {}

// 9XY0
void vx_not_equals_vy(Cpu *cpu) {}

// ANNN
void set_prog_counter(Cpu *cpu) {}

// BNNN
void offset_jump(Cpu *cpu) {}

// CXNN
void random_vx(Cpu *cpu) {}

// DXYN
void draw(Cpu *cpu) {}

// EXNN
void handle_key(Cpu *cpu) {}

// FXNN
void handle_f(Cpu *cpu) {}

