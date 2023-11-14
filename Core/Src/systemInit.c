//**********************************************************************//
// File:        systemInit.c                                            //
// Description: Source file that contains the functions and methods     //
//              implementations for system initializing task            //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last Update: 2023-11-13                                              //
//**********************************************************************//

#include "systemInit.h"

extern Axis xAxes;
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

//**********************************************************************//
// Method:      HAL_UART_RxCpltCallback                                 //
// Description: UART interruption callback                              //
// Parameters:  UART_HandleTypdef *: UART that trigger the interruption //
// Returns:     n/a                                                     //
//**********************************************************************//
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// UART communication wired and bluetooth
	if(huart == &huart3 || huart == &huart7)
		vCommunicationLPUART1Callback();
}


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
		FDCAN_RxHeaderTypeDef RxHeader;
		uint8_t RxData[8];

		// Retrieve RX messages from RX FIFO0
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
			Error_Handler();
		else
			ODrive_RX_CallBack(&xAxes, &RxHeader, RxData);
	}
}
