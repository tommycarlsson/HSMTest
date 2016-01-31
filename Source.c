/************************************************************************
*    Copyright 2016 tommy
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*	   http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
************************************************************************/

#include <stdio.h>
#include "Source.h"
#include <corecrt_malloc.h>


#define HANDLED() StateMachine_handled(t->sm, SM_DUMMY)

static State Motor_off(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("off-SM_ENTRY ");
		t->run = false;
		return HANDLED();
	case SM_EXIT:
		printf("off-SM_EXIT ");
		t->run = true;
		return HANDLED();
	case SM_START:
		printf("off-SM_START ");
		if (t->motor)
		{
			StateMachine_transition(t->sm, t->starting);
		}
		else
		{
			StateMachine_transition(t->sm, t->noMotor);
		}
		return HANDLED();
	default:
		break;
	}

	return StateMachine_topState(t, SM_DUMMY);
}

static State Motor_noMotor(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("noMotor-SM_ENTRY ");
		return HANDLED();
	case SM_EXIT:
		printf("noMotor-SM_EXIT ");
		return HANDLED();
	case SM_STOP:
		printf("noMotor-SM_STOP ");
		StateMachine_transition(t->sm, t->off);
		return HANDLED();
	default:
		break;
	}

	return StateMachine_topState(t, SM_DUMMY);
}

static State Motor_starting(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("starting-SM_ENTRY ");
		return HANDLED();
	case SM_EXIT:
		printf("starting-SM_EXIT ");
		return HANDLED();
	case SM_STOP:
		printf("starting-SM_STOP ");
		StateMachine_transition(t->sm, t->off);
		return HANDLED();
	case SM_PRESENT:
		printf("starting-SM_PRESENT ");
		StateMachine_transition(t->sm, t->powered);
		return HANDLED();
	case SM_BRINGUP_TIMEOUT:
		printf("starting-SM_BRINGUP_TIMEOUT ");
		return HANDLED();
	default:
		break;
	}

	return StateMachine_topState(t, SM_DUMMY);
}

static State Motor_powered(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("powered-SM_ENTRY ");
		return HANDLED();
	case SM_EXIT:
		printf("powered-SM_EXIT ");
		return HANDLED();
	case SM_BRINGUP_TIMEOUT:
		printf("powered-SM_BRINGUP_TIMEOUT ");
		if (t->run)
		{
			StateMachine_transition(t->sm, t->powered);
		}
		else
		{
			StateMachine_transition(t->sm, t->off);
		}
		return HANDLED();
	case SM_STATUS_OK:
		printf("powered-SM_STATUS_OK ");
		StateMachine_transition(t->sm, t->idle);
		return HANDLED();
	case SM_STOP:
		printf("powered-SM_STOP ");
		StateMachine_transition(t->sm, t->off);
		return HANDLED();
    case SM_NOT_PRESENT:
        printf("powered-SM_NOT_PRESENT ");
        if (t->run)
        {
            StateMachine_transition(t->sm, t->starting);
        }
        else
        {
            StateMachine_transition(t->sm, t->off);
        }
        return HANDLED();
	default:
		break;
	}

	return StateMachine_topState(t, SM_DUMMY);
}

static State Motor_idle(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("idle-SM_ENTRY ");
		return HANDLED();
	case SM_EXIT:
		printf("idle-SM_EXIT ");
		return HANDLED();
    case SM_STOP:
        printf("idle-SM_STOP ");
        StateMachine_transition(t->sm, t->off);
        return HANDLED();
    case SM_NOT_PRESENT:
        printf("idle-SM_NOT_PRESENT ");
        StateMachine_transition(t->sm, t->off);
        return HANDLED();
	case SM_TEMP_INQ:
		printf("idle-SM_TEMP_INQ ");
		return HANDLED();
	case SM_PARAM_INQ:
		printf("idle-SM_PARAM_INQ ");
		return HANDLED();
	case SM_PING_INQ:
		printf("idle-SM_PING_INQ ");
		return HANDLED();
	case SM_PARAM_TIMEOUT:
		printf("idle-SM_PARAM_TIMEOUT ");
		return HANDLED();
	case SM_PREA_ON:
		printf("idle-SM_PREA_ON ");
		StateMachine_transition(t->sm, t->preAmpOnOrdered);
		return HANDLED();
	case SM_STATUS_ERR:
		printf("idle-SM_STATUS_ERR ");
		StateMachine_transition(t->sm, t->powered);
		return HANDLED();
	case SM_PING_TIMEOUT:
		printf("idle-SM_PING_TIMEOUT ");
		StateMachine_transition(t->sm, t->powered);
		return HANDLED();
    case SM_STATUS_EE:
        printf("powered-SM_STATUS_EE ");
        StateMachine_transition(t->sm, t->off);
        return HANDLED();
    case SM_STATUS_DE:
        printf("powered-SM_STATUS_DE ");
        StateMachine_transition(t->sm, t->off);
        return HANDLED();
    case SM_STATUS_TEMP:
        printf("powered-SM_STATUS_TEMP ");
        StateMachine_transition(t->sm, t->off);
        return HANDLED();
	default:
		break;
	}

	return StateMachine_topState(t, SM_DUMMY);
}

static State Motor_preAmpOnOrdered(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("preAmpOnOrdered-SM_ENTRY ");
		return HANDLED();
	case SM_EXIT:
		printf("preAmpOnOrdered-SM_EXIT ");
		return HANDLED();
	case SM_STATUS_ED:
		printf("preAmpOnOrdered-SM_STATUS_ED ");
		StateMachine_transition(t->sm, t->armed);
		return HANDLED();
	case SM_PREA_OFF:
		printf("preAmpOnOrdered-SM_PREA_OFF ");
		StateMachine_transition(t->sm, t->idle);
		return HANDLED();
	case SM_TIMEOUT:
		printf("preAmpOnOrdered-SM_TIMEOUT ");
		StateMachine_transition(t->sm, t->idle);
		return HANDLED();
	default:
		break;
	}

	return t->idle;
}

static State Motor_preAmpOffOrdered(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("preAmpOffOrdered-SM_ENTRY ");
		return HANDLED();
	case SM_EXIT:
		printf("preAmpOffOrdered-SM_EXIT ");
		return HANDLED();
	case SM_STATUS_ED:
		printf("preAmpOffOrdered-SM_STATUS_ED ");
		return HANDLED();
	case SM_STATUS_DD:
		printf("preAmpOffOrdered-SM_STATUS_DD ");
		StateMachine_transition(t->sm, t->idle);
		return HANDLED();
	case SM_TIMEOUT:
		printf("preAmpOffOrdered-SM_TIMEOUT ");
		StateMachine_transition(t->sm, t->preAmpOffOrdered);
		return HANDLED();
    case SM_PREA_ON:
        printf("preAmpOffOrdered-SM_PREA_ON ");
        StateMachine_transition(t->sm, t->preAmpOnOrdered);
        return HANDLED();
	default:
		break;
	}

	return t->idle;
}

static State Motor_armed(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("armed-SM_ENTRY ");
		return HANDLED();
	case SM_EXIT:
		printf("armed-SM_EXIT ");
		return HANDLED();
	case SM_STATUS_ED:
		printf("armed-SM_STATUS_ED ");
		return HANDLED();
	case SM_PREA_OFF:
		printf("armed-SM_PREA_OFF ");
		StateMachine_transition(t->sm, t->preAmpOffOrdered);
		return HANDLED();
	case SM_STATUS_DD:
		printf("armed-SM_STATUS_DD ");
		StateMachine_transition(t->sm, t->powered);
		return HANDLED();
	case SM_POWA_ON:
		printf("armed-SM_POWA_ON ");
		StateMachine_transition(t->sm, t->powAmpOnOrdered);
		return HANDLED();
	default:
		break;
	}

	return t->idle;
}

static State Motor_powAmpOnOrdered(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("powAmpOnOrdered-SM_ENTRY ");
		return HANDLED();
	case SM_EXIT:
		printf("powAmpOnOrdered-SM_EXIT ");
		return HANDLED();
	case SM_STATUS_EE:
		printf("powAmpOnOrdered-SM_STATUS_EE ");
		StateMachine_transition(t->sm, t->active);
		return HANDLED();
	case SM_POWA_OFF:
		printf("powAmpOnOrdered-SM_POWA_OFF ");
		StateMachine_transition(t->sm, t->armed);
		return HANDLED();
	case SM_TIMEOUT:
		printf("powAmpOnOrdered-SM_TIMEOUT ");
		StateMachine_transition(t->sm, t->armed);
		return HANDLED();
	default:
		break;
	}

	return t->armed;
}

static State Motor_powAmpOffOrdered(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("powAmpOffOrdered-SM_ENTRY ");
		return HANDLED();
	case SM_EXIT:
		printf("powAmpOffOrdered-SM_EXIT ");
		return HANDLED();
	case SM_STATUS_EE:
		printf("powAmpOffOrdered-SM_STATUS_EE ");
		return HANDLED();
	case SM_STATUS_ED:
		printf("powAmpOffOrdered-SM_STATUS_ED ");
		StateMachine_transition(t->sm, t->armed);
		return HANDLED();
	case SM_TIMEOUT:
		printf("powAmpOffOrdered-SM_TIMEOUT ");
		StateMachine_transition(t->sm, t->powAmpOffOrdered);
		return HANDLED();
	default:
		break;
	}

	return t->armed;
}

static State Motor_active(OWNER owner, Signal e)
{
	Motor* t = owner;

	switch (e)
	{
	case SM_ENTRY:
		printf("active-SM_ENTRY ");
		return HANDLED();
	case SM_EXIT:
		printf("active-SM_EXIT ");
		return HANDLED();
	case SM_STATUS_EE:
		printf("active-SM_STATUS_EE ");
		return HANDLED();
	case SM_POWA_OFF:
		printf("active-SM_PREA_OFF ");
		StateMachine_transition(t->sm, t->powAmpOffOrdered);
		return HANDLED();
	case SM_STATUS_ED:
		printf("active-SM_STATUS_ED ");
		StateMachine_transition(t->sm, t->powAmpOnOrdered);
		return HANDLED();
	default:
		break;
	}

	return t->armed;
}

Motor* Motor_ctor()
{
	Motor* motor = calloc(1, sizeof(Motor));
	if (!motor) return motor;

	motor->off = State_ctor(motor, Motor_off);
	motor->noMotor = State_ctor(motor, Motor_noMotor);
	motor->starting = State_ctor(motor, Motor_starting);
	motor->powered = State_ctor(motor, Motor_powered);
	motor->idle = State_ctor(motor, Motor_idle);
	motor->preAmpOnOrdered = State_ctor(motor, Motor_preAmpOnOrdered);
	motor->preAmpOffOrdered = State_ctor(motor, Motor_preAmpOffOrdered);
	motor->armed = State_ctor(motor, Motor_armed);
	motor->powAmpOnOrdered = State_ctor(motor, Motor_powAmpOnOrdered);
	motor->powAmpOffOrdered = State_ctor(motor, Motor_powAmpOffOrdered);
	motor->active = State_ctor(motor, Motor_active);
	motor->sm = StateMachine_ctor();
	motor->run = false;
	motor->motor = true;

	return motor;
}

void Motor_dtor(Motor* motor)
{
	free(motor);
	motor = NULL;
}

char* stateAsTxt(State s)
{
	if (s->stateFcn_ == Motor_off)   return "off";
	if (s->stateFcn_ == Motor_noMotor)   return "noMotor";
	if (s->stateFcn_ == Motor_starting)  return "starting";
	if (s->stateFcn_ == Motor_powered)   return "powered";
	if (s->stateFcn_ == Motor_idle)  return "idle";
	if (s->stateFcn_ == Motor_preAmpOnOrdered)  return "preAmpOnOrdered";
	if (s->stateFcn_ == Motor_preAmpOffOrdered)  return "preAmpOffOrdered";
	if (s->stateFcn_ == Motor_armed)  return "armed";
	if (s->stateFcn_ == Motor_powAmpOnOrdered)  return "powAmpOnOrdered";
	if (s->stateFcn_ == Motor_powAmpOffOrdered)  return "powAmpOffOrdered";
	if (s->stateFcn_ == Motor_active)  return "active";
	return "Nada";
}
