#ifndef OPCODES_H
#define OPCODES_H

typedef struct Cpu Cpu;

typedef void (* opcode_handler)(Cpu *cpu);

struct Cpu {
	unsigned char memory[4096];
	unsigned char registers[16];
	unsigned short opcode; 
	unsigned short I;
	unsigned short PC;
	unsigned short stack[16];
	unsigned short sp;
	unsigned char key[16];
	opcode_handler handlers[16];
	int (*run_cycle)(Cpu *cpu);
	void (*fetch_opcode)(Cpu *cpu);
	void (*handle_opcode)(Cpu *cpu);
};

// 00EX
void handle_0(Cpu *cpu);

// 00E0
void clear_screen(Cpu *cpu);

// 00EE
void return_from_rsubutine(Cpu *cpu);

// 1NNN
void jump(Cpu *cpur);

// 2NNN
void call_subrutine(Cpu *cpu);

// 3XNN
void vx_equals(Cpu *cpu);

// 4XNN
void vx_not_equals(Cpu *cpu);

// 5XY0
void vx_equals_vy(Cpu *cpu);

// 6XNN
void set_vx(Cpu *cpu);

// 7XNN
void add_to_vx(Cpu *cpu);

// 8XYN
void handle_8(Cpu *cpu);

// 9XY0
void vx_not_equals_vy(Cpu *cpu);

// ANNN
void set_prog_counter(Cpu *cpu);

// BNNN
void offset_jump(Cpu *cpu);

// CXNN
void random_vx(Cpu *cpu);

// DXYN
void draw(Cpu *cpu);

// EXNN
void handle_key(Cpu *cpu);

// FXNN
void handle_f(Cpu *cpu);

#endif 