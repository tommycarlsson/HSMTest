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

#define INVOKE(motor, sig, dst) \
	do { \
	StateMachine_dispatch(motor->sm, sig); \
	assert_true(StateMachine_isInState(motor->sm, dst) == 2); \
	} while (0)

#define IINVOKE(state, sig, dst) \
	do { \
	StateMachine_open(motor->sm, motor, motor->state); \
	assert_true(StateMachine_isInState(motor->sm, motor->state) == 2); \
	StateMachine_dispatch(motor->sm, sig); \
	assert_true(StateMachine_isInState(motor->sm, dst) == 2); \
	} while (0)

static void test_open(void **state)
{
    Motor* motor = *state;
    assert_true(StateMachine_isInState(motor->sm, motor->off) == 2);
}

static void starting(void **state)
{
    Motor* motor = *state;
    StateMachine_open(motor->sm, motor, motor->starting);
    assert_true(StateMachine_isInState(motor->sm, motor->starting) == 2);
}

static void powered(void **state)
{
    Motor* motor = *state;
    StateMachine_open(motor->sm, motor, motor->powered);
    assert_true(StateMachine_isInState(motor->sm, motor->powered) == 2);
}

static void idle(void **state)
{
    Motor* motor = *state;
    StateMachine_open(motor->sm, motor, motor->idle);
    assert_true(StateMachine_isInState(motor->sm, motor->idle) == 2);
}

static void test_off(void **state)
{
    Motor* motor = *state;

    motor->motor = false;
    IINVOKE(off, SM_START, motor->noMotor);
    motor->motor = true;
    IINVOKE(off, SM_START, motor->starting);
    IINVOKE(off, SM_STOP, motor->off);
    IINVOKE(off, SM_BRINGUP_TIMEOUT, motor->off);
    IINVOKE(off, SM_TIMEOUT, motor->off);
    IINVOKE(off, SM_PRESENT, motor->off);
    IINVOKE(off, SM_NOT_PRESENT, motor->off);
    IINVOKE(off, SM_PREA_ON, motor->off);
    IINVOKE(off, SM_PREA_OFF, motor->off);
    IINVOKE(off, SM_POWA_ON, motor->off);
    IINVOKE(off, SM_POWA_OFF, motor->off);
    IINVOKE(off, SM_STATUS_OK, motor->off);
    IINVOKE(off, SM_STATUS_ERR, motor->off);
    IINVOKE(off, SM_STATUS_DD, motor->off);
    IINVOKE(off, SM_STATUS_ED, motor->off);
    IINVOKE(off, SM_STATUS_EE, motor->off);
    IINVOKE(off, SM_STATUS_DE, motor->off);
    IINVOKE(off, SM_STATUS_TEMP, motor->off);
    IINVOKE(off, SM_PING_INQ, motor->off);
    IINVOKE(off, SM_PING_TIMEOUT, motor->off);
    IINVOKE(off, SM_DEVICE_ERR, motor->off);
    IINVOKE(off, SM_PARAM_INQ, motor->off);
    IINVOKE(off, SM_TEMP_INQ, motor->off);
    IINVOKE(off, SM_PARAM_TIMEOUT, motor->off);
    IINVOKE(off, SM_UART_ERR, motor->off);
}

static void test_starting(void **state)
{
    Motor* motor = *state;

    IINVOKE(starting, SM_START, motor->starting);
    IINVOKE(starting, SM_STOP, motor->off);
    motor->run = false;
    IINVOKE(starting, SM_BRINGUP_TIMEOUT, motor->off);
    motor->run = true;
    IINVOKE(starting, SM_BRINGUP_TIMEOUT, motor->starting);
    IINVOKE(starting, SM_TIMEOUT, motor->starting);
    IINVOKE(starting, SM_PRESENT, motor->powered);
    IINVOKE(starting, SM_NOT_PRESENT, motor->starting);
    IINVOKE(starting, SM_PREA_ON, motor->starting);
    IINVOKE(starting, SM_PREA_OFF, motor->starting);
    IINVOKE(starting, SM_POWA_ON, motor->starting);
    IINVOKE(starting, SM_POWA_OFF, motor->starting);
    IINVOKE(starting, SM_STATUS_OK, motor->starting);
    IINVOKE(starting, SM_STATUS_ERR, motor->starting);
    IINVOKE(starting, SM_STATUS_DD, motor->starting);
    IINVOKE(starting, SM_STATUS_ED, motor->starting);
    IINVOKE(starting, SM_STATUS_EE, motor->starting);
    IINVOKE(starting, SM_STATUS_DE, motor->starting);
    IINVOKE(starting, SM_STATUS_TEMP, motor->starting);
    IINVOKE(starting, SM_PING_INQ, motor->starting);
    IINVOKE(starting, SM_PING_TIMEOUT, motor->starting);
    IINVOKE(starting, SM_DEVICE_ERR, motor->starting);
    IINVOKE(starting, SM_PARAM_INQ, motor->starting);
    IINVOKE(starting, SM_TEMP_INQ, motor->starting);
    IINVOKE(starting, SM_PARAM_TIMEOUT, motor->starting);
    IINVOKE(starting, SM_UART_ERR, motor->starting);
}

static void test_noMotor(void **state)
{
    Motor* motor = *state;

    IINVOKE(noMotor, SM_START, motor->noMotor);
    IINVOKE(noMotor, SM_STOP, motor->off);
    IINVOKE(noMotor, SM_BRINGUP_TIMEOUT, motor->noMotor);
    IINVOKE(noMotor, SM_TIMEOUT, motor->noMotor);
    IINVOKE(noMotor, SM_PRESENT, motor->noMotor);
    IINVOKE(noMotor, SM_NOT_PRESENT, motor->noMotor);
    IINVOKE(noMotor, SM_PREA_ON, motor->noMotor);
    IINVOKE(noMotor, SM_PREA_OFF, motor->noMotor);
    IINVOKE(noMotor, SM_POWA_ON, motor->noMotor);
    IINVOKE(noMotor, SM_POWA_OFF, motor->noMotor);
    IINVOKE(noMotor, SM_STATUS_OK, motor->noMotor);
    IINVOKE(noMotor, SM_STATUS_ERR, motor->noMotor);
    IINVOKE(noMotor, SM_STATUS_DD, motor->noMotor);
    IINVOKE(noMotor, SM_STATUS_ED, motor->noMotor);
    IINVOKE(noMotor, SM_STATUS_EE, motor->noMotor);
    IINVOKE(noMotor, SM_STATUS_DE, motor->noMotor);
    IINVOKE(noMotor, SM_STATUS_TEMP, motor->noMotor);
    IINVOKE(noMotor, SM_PING_INQ, motor->noMotor);
    IINVOKE(noMotor, SM_PING_TIMEOUT, motor->noMotor);
    IINVOKE(noMotor, SM_DEVICE_ERR, motor->noMotor);
    IINVOKE(noMotor, SM_PARAM_INQ, motor->noMotor);
    IINVOKE(noMotor, SM_TEMP_INQ, motor->noMotor);
    IINVOKE(noMotor, SM_PARAM_TIMEOUT, motor->noMotor);
    IINVOKE(noMotor, SM_UART_ERR, motor->noMotor);
}

static void test_powered(void **state)
{
    Motor* motor = *state;

    IINVOKE(powered, SM_START, motor->powered);
    IINVOKE(powered, SM_STOP, motor->off);
    motor->run = false;
    IINVOKE(powered, SM_BRINGUP_TIMEOUT, motor->off);
    motor->run = true;
    IINVOKE(powered, SM_BRINGUP_TIMEOUT, motor->starting);
    IINVOKE(powered, SM_TIMEOUT, motor->powered);
    IINVOKE(powered, SM_PRESENT, motor->powered);
    motor->run = false;
    IINVOKE(powered, SM_NOT_PRESENT, motor->off);
    motor->run = true;
    IINVOKE(powered, SM_NOT_PRESENT, motor->starting);
    IINVOKE(powered, SM_PREA_ON, motor->powered);
    IINVOKE(powered, SM_PREA_OFF, motor->powered);
    IINVOKE(powered, SM_POWA_ON, motor->powered);
    IINVOKE(powered, SM_POWA_OFF, motor->powered);
    IINVOKE(powered, SM_STATUS_OK, motor->idle);
    IINVOKE(powered, SM_STATUS_ERR, motor->powered);
    IINVOKE(powered, SM_STATUS_DD, motor->powered);
    IINVOKE(powered, SM_STATUS_ED, motor->powered);
    IINVOKE(powered, SM_STATUS_EE, motor->powered);
    IINVOKE(powered, SM_STATUS_DE, motor->powered);
    IINVOKE(powered, SM_STATUS_TEMP, motor->powered);
    IINVOKE(powered, SM_PING_INQ, motor->powered);
    IINVOKE(powered, SM_PING_TIMEOUT, motor->powered);
    IINVOKE(powered, SM_DEVICE_ERR, motor->powered);
    IINVOKE(powered, SM_PARAM_INQ, motor->powered);
    IINVOKE(powered, SM_TEMP_INQ, motor->powered);
    IINVOKE(powered, SM_PARAM_TIMEOUT, motor->powered);
    IINVOKE(powered, SM_UART_ERR, motor->off);
}

static void test_idle(void **state)
{
    Motor* motor = *state;

    IINVOKE(idle, SM_START,           motor->idle);
    IINVOKE(idle, SM_STOP,            motor->off);
    motor->run = false;
    IINVOKE(idle, SM_BRINGUP_TIMEOUT, motor->off);
    motor->run = true;
    IINVOKE(idle, SM_BRINGUP_TIMEOUT, motor->starting);
    IINVOKE(idle, SM_TIMEOUT,         motor->idle);
    IINVOKE(idle, SM_PRESENT,         motor->idle);
    motor->run = false;
    IINVOKE(idle, SM_NOT_PRESENT,     motor->off);
    motor->run = true;
    IINVOKE(idle, SM_NOT_PRESENT,     motor->starting);
    IINVOKE(idle, SM_PREA_ON,         motor->preAmpOnOrdered);
    IINVOKE(idle, SM_PREA_OFF,        motor->idle);
    IINVOKE(idle, SM_POWA_ON,         motor->idle);
    IINVOKE(idle, SM_POWA_OFF,        motor->idle);
    IINVOKE(idle, SM_STATUS_OK,       motor->idle);
    IINVOKE(idle, SM_STATUS_ERR,      motor->powered);
    IINVOKE(idle, SM_STATUS_DD,       motor->idle);
    IINVOKE(idle, SM_STATUS_ED,       motor->off);
    IINVOKE(idle, SM_STATUS_EE,       motor->off);
    IINVOKE(idle, SM_STATUS_DE,       motor->off);
    IINVOKE(idle, SM_STATUS_TEMP,     motor->off);
    IINVOKE(idle, SM_PING_INQ,        motor->idle);
    IINVOKE(idle, SM_PING_TIMEOUT,    motor->powered);
    IINVOKE(idle, SM_DEVICE_ERR,      motor->off);
    IINVOKE(idle, SM_PARAM_INQ,       motor->idle);
    IINVOKE(idle, SM_TEMP_INQ,        motor->idle);
    IINVOKE(idle, SM_PARAM_TIMEOUT,   motor->idle);
    IINVOKE(idle, SM_UART_ERR,        motor->off);
}

static void test_preAmpOnOrdered(void **state)
{
    Motor* motor = *state;

    IINVOKE(preAmpOnOrdered, SM_START,         motor->preAmpOnOrdered);
    IINVOKE(preAmpOnOrdered, SM_STOP,          motor->off);
    IINVOKE(preAmpOnOrdered, SM_TIMEOUT,       motor->idle);
    IINVOKE(preAmpOnOrdered, SM_PRESENT,       motor->preAmpOnOrdered);
    motor->run = false;
    IINVOKE(preAmpOnOrdered, SM_NOT_PRESENT,   motor->off);
    motor->run = true;
    IINVOKE(preAmpOnOrdered, SM_NOT_PRESENT,   motor->starting);
    IINVOKE(preAmpOnOrdered, SM_PREA_ON,       motor->preAmpOnOrdered);
    IINVOKE(preAmpOnOrdered, SM_PREA_OFF,      motor->idle);
    IINVOKE(preAmpOnOrdered, SM_POWA_ON,       motor->preAmpOnOrdered);
    IINVOKE(preAmpOnOrdered, SM_POWA_OFF,      motor->preAmpOnOrdered);
    IINVOKE(preAmpOnOrdered, SM_STATUS_OK,     motor->preAmpOnOrdered);
    IINVOKE(preAmpOnOrdered, SM_STATUS_ERR,    motor->powered);
    IINVOKE(preAmpOnOrdered, SM_STATUS_DD,     motor->preAmpOnOrdered);
    IINVOKE(preAmpOnOrdered, SM_STATUS_ED,     motor->armed);
    IINVOKE(preAmpOnOrdered, SM_STATUS_EE,     motor->off);
    IINVOKE(preAmpOnOrdered, SM_STATUS_DE,     motor->off);
    IINVOKE(preAmpOnOrdered, SM_STATUS_TEMP,   motor->off);
    IINVOKE(preAmpOnOrdered, SM_PING_INQ,      motor->preAmpOnOrdered);
    IINVOKE(preAmpOnOrdered, SM_PING_TIMEOUT,  motor->powered);
    IINVOKE(preAmpOnOrdered, SM_DEVICE_ERR,    motor->off);
    IINVOKE(preAmpOnOrdered, SM_PARAM_INQ,     motor->preAmpOnOrdered);
    IINVOKE(preAmpOnOrdered, SM_TEMP_INQ,      motor->preAmpOnOrdered);
    IINVOKE(preAmpOnOrdered, SM_PARAM_TIMEOUT, motor->preAmpOnOrdered);
    IINVOKE(preAmpOnOrdered, SM_UART_ERR,      motor->off);
}

static void test_preAmpOffOrdered(void **state)
{
    Motor* motor = *state;

    IINVOKE(preAmpOffOrdered, SM_START,         motor->preAmpOffOrdered);
    IINVOKE(preAmpOffOrdered, SM_STOP,          motor->off);
    IINVOKE(preAmpOffOrdered, SM_TIMEOUT,       motor->idle);
    IINVOKE(preAmpOffOrdered, SM_PRESENT,       motor->preAmpOffOrdered);
    motor->run = false;
    IINVOKE(preAmpOffOrdered, SM_NOT_PRESENT,   motor->off);
    motor->run = true;
    IINVOKE(preAmpOffOrdered, SM_NOT_PRESENT,   motor->starting);
    IINVOKE(preAmpOffOrdered, SM_PREA_ON,       motor->preAmpOnOrdered);
    IINVOKE(preAmpOffOrdered, SM_PREA_OFF,      motor->preAmpOffOrdered);
    IINVOKE(preAmpOffOrdered, SM_POWA_ON,       motor->preAmpOffOrdered);
    IINVOKE(preAmpOffOrdered, SM_POWA_OFF,      motor->preAmpOffOrdered);
    IINVOKE(preAmpOffOrdered, SM_STATUS_OK,     motor->preAmpOffOrdered);
    IINVOKE(preAmpOffOrdered, SM_STATUS_ERR,    motor->powered);
    IINVOKE(preAmpOffOrdered, SM_STATUS_DD,     motor->idle);
    IINVOKE(preAmpOffOrdered, SM_STATUS_ED,     motor->preAmpOffOrdered);
    IINVOKE(preAmpOffOrdered, SM_STATUS_EE,     motor->off);
    IINVOKE(preAmpOffOrdered, SM_STATUS_DE,     motor->off);
    IINVOKE(preAmpOffOrdered, SM_STATUS_TEMP,   motor->off);
    IINVOKE(preAmpOffOrdered, SM_PING_INQ,      motor->preAmpOffOrdered);
    IINVOKE(preAmpOffOrdered, SM_PING_TIMEOUT,  motor->powered);
    IINVOKE(preAmpOffOrdered, SM_DEVICE_ERR,    motor->off);
    IINVOKE(preAmpOffOrdered, SM_PARAM_INQ,     motor->preAmpOffOrdered);
    IINVOKE(preAmpOffOrdered, SM_TEMP_INQ,      motor->preAmpOffOrdered);
    IINVOKE(preAmpOffOrdered, SM_PARAM_TIMEOUT, motor->preAmpOffOrdered);
    IINVOKE(preAmpOffOrdered, SM_UART_ERR,      motor->off);
}

static void test_armed(void **state)
{
    Motor* motor = *state;

    IINVOKE(armed, SM_START,         motor->armed);
    IINVOKE(armed, SM_STOP,          motor->off);
    //IINVOKE(armed, SM_TIMEOUT,       motor->armed);
    IINVOKE(armed, SM_PRESENT,       motor->armed);
    motor->run = false;
    IINVOKE(armed, SM_NOT_PRESENT,   motor->off);
    motor->run = true;
    IINVOKE(armed, SM_NOT_PRESENT,   motor->starting);
    IINVOKE(armed, SM_PREA_ON,       motor->armed);
    IINVOKE(armed, SM_PREA_OFF,      motor->preAmpOffOrdered);
    IINVOKE(armed, SM_POWA_ON,       motor->powAmpOnOrdered);
    IINVOKE(armed, SM_POWA_OFF,      motor->armed);
    IINVOKE(armed, SM_STATUS_OK,     motor->armed);
    IINVOKE(armed, SM_STATUS_ERR,    motor->powered);
    IINVOKE(armed, SM_STATUS_DD,     motor->powered);
    IINVOKE(armed, SM_STATUS_ED,     motor->armed);
    IINVOKE(armed, SM_STATUS_EE,     motor->active);
    IINVOKE(armed, SM_STATUS_DE,     motor->off);
    IINVOKE(armed, SM_STATUS_TEMP,   motor->off);
    IINVOKE(armed, SM_PING_INQ,      motor->armed);
    IINVOKE(armed, SM_PING_TIMEOUT,  motor->powered);
    IINVOKE(armed, SM_DEVICE_ERR,    motor->off);
    IINVOKE(armed, SM_PARAM_INQ,     motor->armed);
    IINVOKE(armed, SM_TEMP_INQ,      motor->armed);
    IINVOKE(armed, SM_PARAM_TIMEOUT, motor->armed);
    IINVOKE(armed, SM_UART_ERR,      motor->off);
}

static void test_powAmpOnOrdered(void **state)
{
    Motor* motor = *state;
}

static void test_powAmpOffOrdered(void **state)
{
    Motor* motor = *state;
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

    IINVOKE(starting, SM_START, motor->starting);
    IINVOKE(starting, SM_STOP, motor->off);
    IINVOKE(starting, SM_TIMEOUT, motor->off);
    IINVOKE(starting, SM_PRESENT, motor->powered);
    IINVOKE(starting, SM_NOT_PRESENT, motor->starting);
    IINVOKE(starting, SM_PREA_ON, motor->starting);
    IINVOKE(starting, SM_PREA_OFF, motor->starting);
    IINVOKE(starting, SM_POWA_ON, motor->starting);
    IINVOKE(starting, SM_POWA_OFF, motor->starting);
    IINVOKE(starting, SM_STATUS_OK, motor->starting);
    IINVOKE(starting, SM_STATUS_ERR, motor->starting);
    IINVOKE(starting, SM_STATUS_DD, motor->starting);
    IINVOKE(starting, SM_STATUS_ED, motor->starting);
    IINVOKE(starting, SM_STATUS_EE, motor->starting);
    IINVOKE(starting, SM_STATUS_DE, motor->starting);
    IINVOKE(starting, SM_STATUS_TEMP, motor->starting);
    IINVOKE(starting, SM_PING_INQ, motor->starting);
    IINVOKE(starting, SM_PING_TIMEOUT, motor->starting);
    IINVOKE(starting, SM_DEVICE_ERR, motor->starting);
    IINVOKE(starting, SM_PARAM_INQ, motor->starting);
    IINVOKE(starting, SM_TEMP_INQ, motor->starting);
    IINVOKE(starting, SM_PARAM_TIMEOUT, motor->starting);
    IINVOKE(starting, SM_UART_ERR, motor->starting);
}

static void test_misc(void **state)
{
    Motor* motor = *state;

    INVOKE(motor, SM_START, motor->starting);
    INVOKE(motor, SM_PRESENT, motor->powered);
    INVOKE(motor, SM_STATUS_OK, motor->idle);
    INVOKE(motor, SM_PREA_ON, motor->preAmpOnOrdered);
    INVOKE(motor, SM_STATUS_ED, motor->armed);
    INVOKE(motor, SM_POWA_ON, motor->powAmpOnOrdered);
    INVOKE(motor, SM_STATUS_EE, motor->active);

    INVOKE(motor, SM_STATUS_ED, motor->powAmpOnOrdered);
    INVOKE(motor, SM_TIMEOUT, motor->armed);
    INVOKE(motor, SM_POWA_ON, motor->powAmpOnOrdered);
    INVOKE(motor, SM_TIMEOUT, motor->armed);
    INVOKE(motor, SM_POWA_OFF, motor->armed);

    INVOKE(motor, SM_POWA_ON, motor->powAmpOnOrdered);
    INVOKE(motor, SM_STATUS_EE, motor->active);
    INVOKE(motor, SM_STATUS_EE, motor->active);
    INVOKE(motor, SM_POWA_OFF, motor->powAmpOffOrdered);
    INVOKE(motor, SM_TIMEOUT, motor->powAmpOffOrdered);
    INVOKE(motor, SM_STATUS_ED, motor->armed);
}

int main(void)
{
    Motor* motor = Motor_ctor();

    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test_setup_teardown(test_open, setup_motor, teardown_motor),
        cmocka_unit_test_setup_teardown(test_starting, setup_motor, teardown_motor),
        cmocka_unit_test_setup_teardown(test_active, setup_motor, teardown_motor),
        cmocka_unit_test_setup_teardown(test_misc, setup_motor, teardown_motor),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}