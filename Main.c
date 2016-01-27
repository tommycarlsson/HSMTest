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
#include <signal.h>
#include "Source.h"


#ifndef TEST_OF_STATEMACHINE
int main(int argc, char* argv[])
{
    Motor* motor = Motor_ctor();
    
    StateMachine_open(motor->sm, motor, motor->off);
    
    for (;;)
    {
        printf("\n%s<-signal:", stateAsTxt(StateMachine_current(motor->sm)));
        
        char c = getc(stdin);
        getc(stdin); // discard '\n'
        
        StateMachine_dispatch(motor->sm, c - '0' + SM_USER_START);
    }
    
	return 0;
}
#endif