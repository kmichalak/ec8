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

static void test_return_decrements_stack_pointer(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->stack[cpu->sp] = cpu->PC;
	cpu->sp += 1;

	cpu->PC = 16;
	cpu->memory[cpu->PC] = 0x00;
	cpu->memory[cpu->PC + 1] = 0xEE;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->sp == 0);

	test_free(cpu);
}

static void test_return_pops_stack_to_pc_and_increments_ps(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	unsigned short initial_pc = cpu->PC;

	cpu->stack[cpu->sp] = cpu->PC;
	cpu->sp += 1;

	cpu->PC = 16;
	cpu->memory[cpu->PC] = 0x00;
	cpu->memory[cpu->PC + 1] = 0xEE;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->PC == initial_pc + 2);

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

static void test_skips_one_instruction_when_cx_equals_nn(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	const char expected_registers[16] = {
		0x00, 0x12, 0x13, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
	};

	cpu->registers[1] = 0x12;

	cpu->memory[cpu->PC] = 0x31;
	cpu->memory[cpu->PC + 1] = 0x12;

	cpu->memory[cpu->PC + 2] = 0x61;
	cpu->memory[cpu->PC + 3] = 0x66;

	cpu->memory[cpu->PC + 4] = 0x62;
	cpu->memory[cpu->PC + 5] = 0x13;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_memory_equal(expected_registers, cpu->registers, 
		sizeof(expected_registers));

	test_free(cpu);
}

static void test_executes_next_instruction_when_cx_not_equal_to_nn(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	const char expected_registers[16] = {
		0x00, 0x66, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
	};

	cpu->memory[cpu->PC] = 0x31;
	cpu->memory[cpu->PC + 1] = 0x12;

	cpu->memory[cpu->PC + 2] = 0x61;
	cpu->memory[cpu->PC + 3] = 0x66;

	cpu->memory[cpu->PC + 4] = 0x62;
	cpu->memory[cpu->PC + 5] = 0x13;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_memory_equal(expected_registers, cpu->registers, 
		sizeof(expected_registers));

	test_free(cpu);
}

static void test_skips_one_instruction_when_cx_not_equal_to_nn(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	const char expected_registers[16] = {
		0x00, 0x00, 0x13, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
	};

	cpu->memory[cpu->PC] = 0x41;
	cpu->memory[cpu->PC + 1] = 0x12;

	cpu->memory[cpu->PC + 2] = 0x61;
	cpu->memory[cpu->PC + 3] = 0x66;

	cpu->memory[cpu->PC + 4] = 0x62;
	cpu->memory[cpu->PC + 5] = 0x13;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_memory_equal(expected_registers, cpu->registers, 
		sizeof(expected_registers));

	test_free(cpu);
}

static void test_executes_next_instruction_when_cx_equal_to_nn(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	const char expected_registers[16] = {
		0x00, 0x66, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
	};

	cpu->registers[1] = 0x12;

	cpu->memory[cpu->PC] = 0x41;
	cpu->memory[cpu->PC + 1] = 0x12;

	cpu->memory[cpu->PC + 2] = 0x61;
	cpu->memory[cpu->PC + 3] = 0x66;

	cpu->memory[cpu->PC + 4] = 0x62;
	cpu->memory[cpu->PC + 5] = 0x13;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_memory_equal(expected_registers, cpu->registers, 
		sizeof(expected_registers));

	test_free(cpu);
}

static void test_skips_one_instruction_when_vx_not_eq_to_vy(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	const char expected_registers[16] = {
		0x00, 0x12, 0x13, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
	};

	cpu->registers[1] = 0x12;

	cpu->memory[cpu->PC] = 0x91;
	cpu->memory[cpu->PC + 1] = 0x20;

	cpu->memory[cpu->PC + 2] = 0x61;
	cpu->memory[cpu->PC + 3] = 0x66;

	cpu->memory[cpu->PC + 4] = 0x62;
	cpu->memory[cpu->PC + 5] = 0x13;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_memory_equal(expected_registers, cpu->registers, 
		sizeof(expected_registers));

	test_free(cpu);
}

static void test_executes_next_instruction_when_vx_eq_vy(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	const char expected_registers[16] = {
		0x00, 0x66, 0x12, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
	};

	cpu->registers[1] = 0x12;
	cpu->registers[2] = 0x12;

	cpu->memory[cpu->PC] = 0x91;
	cpu->memory[cpu->PC + 1] = 0x20;

	cpu->memory[cpu->PC + 2] = 0x61;
	cpu->memory[cpu->PC + 3] = 0x66;

	cpu->memory[cpu->PC + 4] = 0x62;
	cpu->memory[cpu->PC + 5] = 0x13;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_memory_equal(expected_registers, cpu->registers, 
		sizeof(expected_registers));

	test_free(cpu);
}

static void test_ANNN_sets_I_to_NNN(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0xA1;
	cpu->memory[cpu->PC + 1] = 0x23;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->I == 0x0123);

	test_free(cpu);
}

static void test_ANNN_increases_PC(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	unsigned short initial_pc = cpu->PC;
	cpu->memory[cpu->PC] = 0xA1;
	cpu->memory[cpu->PC + 1] = 0x23;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->PC == initial_pc + 2);

	test_free(cpu);
}

static void test_BNNN_jumps_to_address_NNN_with_vx_offset(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0xB7;
	cpu->memory[cpu->PC+1] = 0xBC;
	
	cpu->registers[0x0] = 0x011;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->PC == 0x07CD);

	test_free(cpu);	
}

static void test_CXNN_sets_vx_to_NN_with_random_mask(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0xCA;
	cpu->memory[cpu->PC+1] = 0xBC;
	
	cpu->registers[0x0] = 0x011;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x0A] != 0);
	assert_true(cpu->registers[0x0A] != 0x00BC);

	test_free(cpu);	
}

static void test_8XY0_stores_vy_to_vx(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC+1] = 0x20;
	
	cpu->registers[0x1] = 0x088;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x01] == 0x088);
	assert_true(cpu->registers[0x02] == 0x088);

	test_free(cpu);		
}

int main(int argc, char **argv) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_increments_pc_after_opcode_handle),
		cmocka_unit_test(test_return_decrements_stack_pointer),
		cmocka_unit_test(test_return_pops_stack_to_pc_and_increments_ps),
		cmocka_unit_test(test_add_to_vx_can_add_to_all_registers),
		cmocka_unit_test(test_set_vx_can_write_to_all_registers),
		cmocka_unit_test(test_jump_sets_program_counter_to_address),
		cmocka_unit_test(test_call_subroutine_increments_stack_pointer),
		cmocka_unit_test(test_call_subroutine_puts_current_address_in_stack),
		cmocka_unit_test(test_call_subroutine_sets_program_counter),
		cmocka_unit_test(test_calls_subroutine),
		cmocka_unit_test(test_skips_one_instruction_when_cx_equals_nn),
		cmocka_unit_test(test_executes_next_instruction_when_cx_not_equal_to_nn),
		cmocka_unit_test(test_skips_one_instruction_when_cx_not_equal_to_nn),
		cmocka_unit_test(test_executes_next_instruction_when_cx_equal_to_nn),
		cmocka_unit_test(test_skips_one_instruction_when_vx_not_eq_to_vy),
		cmocka_unit_test(test_executes_next_instruction_when_vx_eq_vy),
		cmocka_unit_test(test_ANNN_sets_I_to_NNN),
		cmocka_unit_test(test_ANNN_increases_PC),
		cmocka_unit_test(test_BNNN_jumps_to_address_NNN_with_vx_offset),
		cmocka_unit_test(test_CXNN_sets_vx_to_NN_with_random_mask),
		cmocka_unit_test(test_8XY0_stores_vy_to_vx)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}