//**********************************************************************//
// File:        systemInit.c                                            //
// Description: Source file that contains the functions and methods     //
//              implementations for system initializing task            //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last Update: 2023-11-13                                              //
//**********************************************************************//

#include "systemInit.h"

extern osThreadId_t systemControlHandle;

//**********************************************************************//
// Method:      vSystemInitStart                                        //
// Description: Entrypoint to line follower robot initializing task     //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vSystemInitStart()
{
	vSensorReadingsInit();
	vCommunicationInit(&huart3, &huart7);

	HAL_TIM_Base_Start_IT(&htim7);  		// System and IMU updates

	osThreadFlagsSet(systemControlHandle, 0b1);

	vActuatorConfigSin(0.1, 8*3.14/181, 0, 0);
	vActuatorPositionControlStart(SIN);

	while(1)
	{
		vActuadorRun();
		osDelay(10);
	}
}
