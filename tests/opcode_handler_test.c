#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>

#include "../cpu.h"

static void test_increments_pc_after_opcode_handle(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x61;
	cpu->memory[cpu->PC + 1] = 0xAA;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->PC == 2);

	test_free(cpu);
}

static void test_set_vx_can_write_to_all_registers(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));

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
	
	test_free(cpu);
}

static void test_add_to_vx_can_add_to_all_registers(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));

	const char expected_registers[16] = {
		0x01, 0x02, 0x03, 0x04,
		0x05, 0x06, 0x07, 0x08,
		0x09, 0x0A, 0x0B, 0x0C,
		0x0D, 0x0E, 0x0F, 0x10,
	};

	initialize(cpu);

	for (int i=0; i < 16; i++) {
		char reg_val = expected_registers[i];

		cpu->memory[cpu->PC] = 0x70 | i;
		cpu->memory[cpu->PC + 1] = reg_val;

		cpu->fetch_opcode(cpu);
		cpu->handle_opcode(cpu);
	}

	assert_memory_equal(expected_registers, cpu->registers, 
						sizeof(expected_registers));

	test_free(cpu);
}

static void test_jump_sets_program_counter_to_address(void **state)  {
	Cpu *cpu = test_malloc(sizeof(Cpu));

	initialize(cpu);
	cpu->memory[cpu->PC] = 0x1A;
	cpu->memory[cpu->PC+1] = 0xBC;
	
	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->PC == 0x0ABC);

	test_free(cpu);
}

static void test_call_subroutine_increments_stack_pointer(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));

	initialize(cpu);

	cpu->memory[cpu->PC] = 0x2A;
	cpu->memory[cpu->PC + 1] = 0xAA;

	cpu->memory[0xAAA] = 0x61;
	cpu->memory[0xAAB] = 0xEE;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->sp == 1);

	test_free(cpu);
}

static void test_call_subroutine_puts_current_address_in_stack(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));

	initialize(cpu);

	unsigned short initial_pc = 1;
	cpu->PC += initial_pc;
	cpu->memory[cpu->PC] = 0x2A;
	cpu->memory[cpu->PC + 1] = 0xAA;

	cpu->memory[0xAAA] = 0x61;
	cpu->memory[0xAAB] = 0xEE;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->stack[cpu->sp - 1] == initial_pc);

	test_free(cpu);
}

static void test_call_subroutine_sets_program_counter(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));

	initialize(cpu);

	cpu->PC += 1;
	cpu->memory[cpu->PC] = 0x2A;
	cpu->memory[cpu->PC + 1] = 0xAA;

	cpu->memory[0xAAA] = 0x61;
	cpu->memory[0xAAB] = 0xEE;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->PC == 0x0AAA + 0x02);

	test_free(cpu);
}

static void test_calls_subroutine(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));

	initialize(cpu);

	cpu->PC += 1;
	cpu->memory[cpu->PC] = 0x2A;
	cpu->memory[cpu->PC + 1] = 0xAA;

	cpu->memory[0xAAA] = 0x61;
	cpu->memory[0xAAB] = 0xEE;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x01] == 0xEE);

	test_free(cpu);
}

int main(int argc, char **argv) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_increments_pc_after_opcode_handle),
		cmocka_unit_test(test_add_to_vx_can_add_to_all_registers),
		cmocka_unit_test(test_set_vx_can_write_to_all_registers),
		cmocka_unit_test(test_jump_sets_program_counter_to_address),
		cmocka_unit_test(test_call_subroutine_increments_stack_pointer),
		cmocka_unit_test(test_call_subroutine_puts_current_address_in_stack),
		cmocka_unit_test(test_call_subroutine_sets_program_counter),
		cmocka_unit_test(test_calls_subroutine)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}