#include <stdio.h>
#include <signal.h>
#include "StateMachineC.h"

#define HANDLED() StateMachine_handled(t->sm, SM_DUMMY)

typedef struct
{
	State off;
	State noMotor;
    State starting;
    State powered;
    State idle;
    State preAmpOnOrdered;
    State preAmpOffOrdered;
    State armed;
    State powAmpOnOrdered;
    State powAmpOffOrdered;
    State active;
    StateMachine sm;
    int run;
    bool motor;
} Tester;

typedef enum
{
    SM_START = SM_USER_START,
    SM_STOP,
    SM_TIMEOUT,
    SM_PRESENT,
    SM_NOT_PRESENT,
    SM_PREA_ON,
    SM_PREA_OFF,
    SM_POWA_ON,
    SM_POWA_OFF,
    SM_STATUS_OK,
	SM_STATUS_ERR, 
    SM_STATUS_DD,
    SM_STATUS_ED,
    SM_STATUS_EE,
    SM_STATUS_DE,
    SM_STATUS_TEMP,
    SM_PING_INQ,
    SM_PING_TIMEOUT,
    SM_DEVICE_ERR,
    SM_TAKE_INQ,
    SM_TEMP_INQ,
    SM_PARAM_TIMEOUT,
    SM_UART_ERR,
} TesterSignals;

State Tester_off(OWNER owner, Signal e)
{
    Tester* t = owner;
    
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

State Tester_noMotor(OWNER owner, Signal e)
{
    Tester* t = owner;
    
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

State Tester_starting(OWNER owner, Signal e)
{
    Tester* t = owner;
    
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
        case SM_TIMEOUT:
            printf("starting-SM_TIMEOUT ");
            return HANDLED();
        default:
            break;
    }
    
    return StateMachine_topState(t, SM_DUMMY);
}

State Tester_powered(OWNER owner, Signal e)
{
    Tester* t = owner;
    
    switch (e)
    {
        case SM_ENTRY:
            printf("powered-SM_ENTRY ");
            return HANDLED();
        case SM_EXIT:
            printf("powered-SM_EXIT ");
            return HANDLED();
        case SM_STATUS_OK:
            printf("powered-SM_STATUS_OK ");
            StateMachine_transition(t->sm, t->idle);
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
        case SM_STATUS_ERR:
            printf("powered-SM_STATUS_ERR ");
            StateMachine_transition(t->sm, t->off);
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
            } else
            {
                StateMachine_transition(t->sm, t->off);
            }
            return HANDLED();
        default:
            break;
    }
    
    return StateMachine_topState(t, SM_DUMMY);
}

State Tester_idle(OWNER owner, Signal e)
{
    Tester* t = owner;
    
    switch (e)
    {
        case SM_ENTRY:
            printf("idle-SM_ENTRY ");
            return HANDLED();
        case SM_EXIT:
            printf("idle-SM_EXIT ");
            return HANDLED();
        case SM_TEMP_INQ:
            printf("idle-SM_TEMP_INQ ");
            return HANDLED();
        case SM_TAKE_INQ:
            printf("idle-SM_TAKE_INQ ");
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
        default:
            break;
    }
    
    return t->powered;
}

State Tester_preAmpOnOrdered(OWNER owner, Signal e)
{
    Tester* t = owner;
    
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

State Tester_preAmpOffOrdered(OWNER owner, Signal e)
{
    Tester* t = owner;
    
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
        default:
            break;
    }
    
    return t->idle;
}

State Tester_armed(OWNER owner, Signal e)
{
    Tester* t = owner;
    
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

State Tester_powAmpOnOrdered(OWNER owner, Signal e)
{
    Tester* t = owner;
    
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

State Tester_powAmpOffOrdered(OWNER owner, Signal e)
{
    Tester* t = owner;
    
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

State Tester_active(OWNER owner, Signal e)
{
    Tester* t = owner;
    
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
            StateMachine_transition(t->sm, t->powered);
            return HANDLED();
        default:
            break;
    }
    
    return t->armed;
}

char* stateAsTxt(State s)
{
    if (s->stateFcn_ == Tester_off)   return "off";
    if (s->stateFcn_ == Tester_noMotor)   return "noMotor";
    if (s->stateFcn_ == Tester_starting)  return "starting";
    if (s->stateFcn_ == Tester_powered)   return "powered";
    if (s->stateFcn_ == Tester_idle)  return "idle";
    if (s->stateFcn_ == Tester_preAmpOnOrdered)  return "preAmpOnOrdered";
    if (s->stateFcn_ == Tester_preAmpOffOrdered)  return "preAmpOffOrdered";
    if (s->stateFcn_ == Tester_armed)  return "armed";
    if (s->stateFcn_ == Tester_powAmpOnOrdered)  return "powAmpOnOrdered";
    if (s->stateFcn_ == Tester_powAmpOffOrdered)  return "powAmpOffOrdered";
    if (s->stateFcn_ == Tester_active)  return "active";
    return "Nada";
}

int main(int argc, char* argv[])
{
    Tester tester;
    
	tester.off   = State_ctor(&tester, Tester_off);
	tester.noMotor   = State_ctor(&tester, Tester_noMotor);
    tester.starting  = State_ctor(&tester, Tester_starting);
    tester.powered   = State_ctor(&tester, Tester_powered);
    tester.idle  = State_ctor(&tester, Tester_idle);
    tester.preAmpOnOrdered = State_ctor(&tester, Tester_preAmpOnOrdered);
    tester.preAmpOffOrdered = State_ctor(&tester, Tester_preAmpOffOrdered);
    tester.preAmpOnOrdered = State_ctor(&tester, Tester_armed);
    tester.powAmpOnOrdered = State_ctor(&tester, Tester_powAmpOnOrdered);
    tester.powAmpOffOrdered = State_ctor(&tester, Tester_powAmpOffOrdered);
    tester.active = State_ctor(&tester, Tester_active);
    tester.sm = StateMachine_ctor();
    tester.run = false;
    tester.motor = true;
    
    StateMachine_open(tester.sm, &tester, tester.off);
    
    for (;;)
    {
        printf("\n%s<-signal:", stateAsTxt(StateMachine_current(tester.sm)));
        
        char c = getc(stdin);
        getc(stdin); // discard '\n'
        
        StateMachine_dispatch(tester.sm, c - '0' + SM_USER_START);
    }
    
	return 0;
}