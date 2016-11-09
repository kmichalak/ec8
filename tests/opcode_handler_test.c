#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>

#include "../cpu.h"

static void test_set_vx_can_write_to_all_registers(void **state) {
	Cpu *cpu = malloc(sizeof(Cpu));

	const char expected_registers[16] = {
		0x01, 0x02, 0x03, 0x04,
		0x05, 0x06, 0x07, 0x08,
		0x09, 0x0A, 0x0B, 0x0C,
		0x0D, 0x0E, 0x0F, 0x10,
	};

	initialize(cpu);

	for (int i=0; i < 16; i++) {
		char reg_val = expected_registers[i];
		cpu->memory[cpu->PC] = 0x60 | i;
		cpu->memory[cpu->PC + 1] = reg_val;

		cpu->fetch_opcode(cpu);
		cpu->handle_opcode(cpu);
	}

	assert_memory_equal(expected_registers, cpu->registers, 
						sizeof(expected_registers));

	free(cpu);
}

static void test_add_to_vx_can_add_to_all_registers(void **state) {
	Cpu *cpu1 = malloc(sizeof(Cpu));

	const char expected_registers1[16] = {
		0x01, 0x02, 0x03, 0x04,
		0x05, 0x06, 0x07, 0x08,
		0x09, 0x0A, 0x0B, 0x0C,
		0x0D, 0x0E, 0x0F, 0x10,
	};

	initialize(cpu1);

	for (int i=0; i < 16; i++) {
		char reg_val = expected_registers1[i];
		cpu1->memory[cpu1->PC] = 0x70 | i;
		cpu1->memory[cpu1->PC + 1] = reg_val;

		cpu1->fetch_opcode(cpu1);
		cpu1->handle_opcode(cpu1);
	}

	assert_memory_equal(expected_registers1, cpu1->registers, 
						sizeof(expected_registers1));

	free(cpu1);
}

static void test_jump_sets_program_counter_to_address(void **state)  {
	Cpu *cpu = malloc(sizeof(Cpu));

	initialize(cpu);
	cpu->memory[cpu->PC] = 0x1A;
	cpu->memory[cpu->PC+1] = 0xBC;
	
	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->PC == 0x0ABC);

	free(cpu);
}

int main(int argc, char **argv) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_add_to_vx_can_add_to_all_registers),
		cmocka_unit_test(test_set_vx_can_write_to_all_registers),
		cmocka_unit_test(test_jump_sets_program_counter_to_address)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}