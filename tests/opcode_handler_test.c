#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>

#include "../cpu.h"

static void test_set_vx_value(void **state) {
	Cpu *cpu = malloc(sizeof(Cpu));

	initialize(cpu);

	cpu->memory[cpu->PC] = 0x61;
	cpu->memory[cpu->PC + 1] = 0x34;

	cpu->fetch_opcode(cpu);
	cpu->handle_opcode(cpu);

	assert_true(cpu->registers[1] == 0x34);

	free(cpu);
}

int main(int argc, char **argv) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_set_vx_value)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}