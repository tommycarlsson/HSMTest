#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../Source.h"
#include "../StateMachineC.h"

static int setup_motor(void **state)
{
	Motor* motor = Motor_ctor();
	StateMachine_open(motor->sm, motor, motor->off);
	*state = (void*)motor;
	return 0;
}

static int teardown_motor(void **state)
{
	Motor* motor = *state;
	Motor_dtor(motor);
	return 0;
}

static void test_open(void **state)
{
	Motor* motor = *state;
	assert_true(StateMachine_isInState(motor->sm, motor->off) == 2);
}

static void test_starting(void **state)
{
	Motor* motor = *state;

	StateMachine_dispatch(motor->sm, SM_START);
	assert_true(StateMachine_isInState(motor->sm, motor->starting) == 2);
}

static void test_active(void **state)
{
	Motor* motor = *state;

	StateMachine_dispatch(motor->sm, SM_START);
	assert_true(StateMachine_isInState(motor->sm, motor->starting) == 2);
	StateMachine_dispatch(motor->sm, SM_PRESENT);
	assert_true(StateMachine_isInState(motor->sm, motor->powered) == 2);
	StateMachine_dispatch(motor->sm, SM_STATUS_OK);
	assert_true(StateMachine_isInState(motor->sm, motor->idle) == 2);
	StateMachine_dispatch(motor->sm, SM_PREA_ON);
	assert_true(StateMachine_isInState(motor->sm, motor->preAmpOnOrdered) == 2);
	StateMachine_dispatch(motor->sm, SM_STATUS_ED);
	assert_true(StateMachine_isInState(motor->sm, motor->armed) == 2);
	StateMachine_dispatch(motor->sm, SM_POWA_ON);
	assert_true(StateMachine_isInState(motor->sm, motor->powAmpOnOrdered) == 2);
	StateMachine_dispatch(motor->sm, SM_STATUS_EE);
	assert_true(StateMachine_isInState(motor->sm, motor->active) == 2);
}

int main(void)
{
	Motor* motor = Motor_ctor();

	const struct CMUnitTest tests[] =
	{
		cmocka_unit_test_setup_teardown(test_open, setup_motor, teardown_motor),
		cmocka_unit_test_setup_teardown(test_starting, setup_motor, teardown_motor),
		cmocka_unit_test_setup_teardown(test_active, setup_motor, teardown_motor),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}