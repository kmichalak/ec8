#ifndef CK8_H
#define CK8_H

// General description of CHIP-8 can be found at 
// https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description.

#define V0 0x00
#define V1 0x01
#define V2 0x02
#define V3 0x03
#define V4 0x04
#define V5 0x05
#define V6 0x06
#define V7 0x07
#define V8 0x08
#define V9 0x09
#define VA 0x0A
#define VB 0x0B
#define VC 0x0C
#define VD 0x0D
#define VE 0x0E
#define VF 0x0F

typedef struct Cpu {
	unsigned char memory[4096];
	unsigned char registers[VF];
	unsigned short opcode; 
	unsigned short I;
	unsigned short PC;
	unsigned short stack[16];
	unsigned short sp;
	unsigned char key[16];
} Cpu;

typedef void (* opcode_handler)(unsigned short opcode);

// typedef opcode_handler opcode_handlers[16];


#endif