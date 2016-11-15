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

static void test_run_next_instruction_when_cx_not_equal_to_nn(void **state) {
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

	cpu->registers[0x2] = 0x088;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x01] == 0x088);
	assert_true(cpu->registers[0x02] == 0x088);

	test_free(cpu);
}

static void test_8XY1_sets_vx_to_vx_or_vy(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC+1] = 0x21;

	cpu->registers[0x1] = 0x12;
	cpu->registers[0x2] = 0xf0;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x01] == (0x12 | 0xf0));

	test_free(cpu);
}

static void test_8XY2_sets_vx_to_vx_and_vy(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC+1] = 0x22;

	cpu->registers[0x1] = 0x12;
	cpu->registers[0x2] = 0xf0;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x01] == (0x12 & 0xf0));

	test_free(cpu);
}

static void test_8XY2_sets_vx_to_vx_xor_vy(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC+1] = 0x23;

	cpu->registers[0x1] = 0x12;
	cpu->registers[0x2] = 0xf0;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x01] == (0x12 ^ 0xf0));

	test_free(cpu);
}

static void test_8XY4_sets_vx_to_vx_plus_vy(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC+1] = 0x24;

	cpu->registers[0x1] = 0x08;
	cpu->registers[0x2] = 0x07;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x01] == 0x08 + 0x07);

	test_free(cpu);
}

static void test_8XY4_sets_carry_flag_in_vf(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC+1] = 0x24;

	cpu->registers[0x1] = 0x12;
	cpu->registers[0x2] = 0xf0;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0xf] == 1);

	test_free(cpu);
}

static void test_8XY4_does_not_set_carry_flag(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC+1] = 0x24;

	cpu->registers[0x1] = 0x12;
	cpu->registers[0x2] = 0x45;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0xf] == 0);

	test_free(cpu);
}

static void test_8XY5_sets_vx_to_vx_sub_vy(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC + 1] = 0x25;

	cpu->registers[0x1] = 0x45;
	cpu->registers[0x2] = 0x12;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x1] == 0x45 - 0x12);

	test_free(cpu);
}

static void test_8XY5_sets_sets_borrow_flag_when_vx_gt_vy(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC + 1] = 0x25;

	cpu->registers[0x1] = 0x45;
	cpu->registers[0x2] = 0x12;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0xf] == 0x1);

	test_free(cpu);
}


static void test_8XY5_clears_borrow_flag_when_no_borrow(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC + 1] = 0x25;

	cpu->registers[0x1] = 0x12;
	cpu->registers[0x2] = 0x45;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0xf] == 0x0);

	test_free(cpu);
}

static void test_8xy6_shifts_vx_by_one(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC + 1] = 0x16;

	cpu->registers[0x1] = 0x08;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x1] == 0x4);

	test_free(cpu);
}

static void test_8xy6_sets_vf_when_lsb_is_one(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC + 1] = 0x16;

	cpu->registers[0x1] = 0x09;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0xf] == 0x1);

	test_free(cpu);
}

static void test_8xy6_clears_vf_when_lsb_is_zero(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC + 1] = 0x16;

	cpu->registers[0x1] = 0x08;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0xf] == 0x0);

	test_free(cpu);
}

static void test_8xy7_sets_vx_to_vy_sub_vx(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC + 1] = 0x27;

	cpu->registers[0x1] = 0x12;
	cpu->registers[0x2] = 0x45;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x01] == 0x45 - 0x12);

	test_free(cpu);
}

static void test_8xy7_sets_vf_when_no_borrow(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC + 1] = 0x27;

	cpu->registers[0x1] = 0x12;
	cpu->registers[0x2] = 0x45;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0xf] == 0x1);

	test_free(cpu);
}

static void test_8xy7_clears_vf_when_borrow(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x82;
	cpu->memory[cpu->PC + 1] = 0x17;

	cpu->registers[0x1] = 0x12;
	cpu->registers[0x2] = 0x45;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0xf] == 0x0);

	test_free(cpu);
}

static void test_8xyE_shifts_vx_left_by_one(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC + 1] = 0x1e;

	cpu->registers[0x1] = 0x4;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0x1] == 0x8);

	test_free(cpu);
}

static void test_8xyE_stores_msb_in_vf(void **state) {
	Cpu *cpu = test_malloc(sizeof(Cpu));
	initialize(cpu);

	cpu->memory[cpu->PC] = 0x81;
	cpu->memory[cpu->PC + 1] = 0x1e;

	cpu->registers[0x1] = 0xe3;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[0xf] == 0x1);

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
		cmocka_unit_test(test_run_next_instruction_when_cx_not_equal_to_nn),
		cmocka_unit_test(test_skips_one_instruction_when_cx_not_equal_to_nn),
		cmocka_unit_test(test_executes_next_instruction_when_cx_equal_to_nn),
		cmocka_unit_test(test_skips_one_instruction_when_vx_not_eq_to_vy),
		cmocka_unit_test(test_executes_next_instruction_when_vx_eq_vy),
		cmocka_unit_test(test_ANNN_sets_I_to_NNN),
		cmocka_unit_test(test_ANNN_increases_PC),
		cmocka_unit_test(test_BNNN_jumps_to_address_NNN_with_vx_offset),
		cmocka_unit_test(test_CXNN_sets_vx_to_NN_with_random_mask),
		cmocka_unit_test(test_8XY0_stores_vy_to_vx),
		cmocka_unit_test(test_8XY1_sets_vx_to_vx_or_vy),
		cmocka_unit_test(test_8XY2_sets_vx_to_vx_and_vy),
		cmocka_unit_test(test_8XY2_sets_vx_to_vx_xor_vy),
		cmocka_unit_test(test_8XY4_sets_vx_to_vx_plus_vy),
		cmocka_unit_test(test_8XY4_sets_carry_flag_in_vf),
		cmocka_unit_test(test_8XY4_does_not_set_carry_flag),
		cmocka_unit_test(test_8XY5_sets_vx_to_vx_sub_vy),
		cmocka_unit_test(test_8XY5_sets_sets_borrow_flag_when_vx_gt_vy),
		cmocka_unit_test(test_8XY5_clears_borrow_flag_when_no_borrow),
		cmocka_unit_test(test_8xy6_shifts_vx_by_one),
		cmocka_unit_test(test_8xy6_sets_vf_when_lsb_is_one),
		cmocka_unit_test(test_8xy6_clears_vf_when_lsb_is_zero),
		cmocka_unit_test(test_8xy7_sets_vx_to_vy_sub_vx),
		cmocka_unit_test(test_8xy7_sets_vf_when_no_borrow),
		cmocka_unit_test(test_8xy7_clears_vf_when_borrow),
		cmocka_unit_test(test_8xyE_shifts_vx_left_by_one),
		cmocka_unit_test(test_8xyE_stores_msb_in_vf)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}