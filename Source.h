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

#pragma once

#include "StateMachineC.h"

#ifdef __cplusplus
extern "C" {
#endif

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
	} Motor;

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
	} MotorSignals;

	Motor* Motor_ctor();
	void Motor_dtor(Motor* motor);

	char* stateAsTxt(State s);

#ifdef __cplusplus
}
#endif