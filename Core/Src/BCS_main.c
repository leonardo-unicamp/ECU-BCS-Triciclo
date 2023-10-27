//**********************************************************************//
// File:        BCS_main.c                                              //
// Description: bare metal code that execute that PID of the balance    //
//              control system and signal acquisition of the cycle-car  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-10-17                                              //
//**********************************************************************//

#include "BCS_main.h"

timerFlags_t timFlags;
Axis Axes[MAX_AXES];
SCHA63TOutput SCHA63TData;

float fLastMotorSpeed;
float fMotorAcceleration;
float fActuadorTorque;
float fAccelerationMeasured;
float fAccelerationAngle;

extern pid_data_type pidConfig;


//**********************************************************************//
// Method:      vBCSInit                                                //
// Description: Initializes the peripherals necessary for the balance   //
//              control system to function                              //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vBCSInit(void)
{
	// initialize IMU device
	char cImuInit = cSCHA63TInit(
						IMU1_CS_UNO_GPIO_Port,
						IMU1_CS_UNO_Pin,
						IMU1_CS_DUE_GPIO_Port,
						IMU1_CS_DUE_Pin,
						&hspi4
					);
	if (!cImuInit)
		  HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);

	// Update gyroscopes offset
	cSCHA63TOffsetCalculation();

	//Initialize Axis Id and CAN Instance
	Axes[0].AxisID = 2;
	Axes[0].CanInstance = &hfdcan1;
	Axes[1].AxisID = 3;
	Axes[1].CanInstance = &hfdcan1;
	CAN_Setup(Axes, 1);

	//Initialize Motor Control Driver (ODrive)
	Set_Input_Pos(Axes[0], 0, 0, 0);
	Reboot_ODrive(Axes[0]);
	HAL_Delay(3000);
	Set_Limits(Axes[0], 96, 120); //96 turns/s, 120 A
	HAL_Delay(10);
	Set_Controller_Modes(Axes[0], TORQUE_CONTROL, PASSTHROUGH);
	HAL_Delay(10);
	Set_Axis_Requested_State(Axes[0], CLOSED_LOOP_CONTROL);
	HAL_Delay(10);

	// Initialize communication state machine (Wired and Bluetooth)
	vCommunicationInit(&huart3, &huart7);

	// Initialize timers
	HAL_TIM_Base_Start_IT(&htim7);  // System and IMU updates
	HAL_TIM_Base_Start_IT(&htim6);  // Communication state machine

	// Initialize PID BCS control
	pid_init(64, 37, 2.0, 400, 200);

	// Turn on led that indicate that system has already initialized
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
}


//**********************************************************************//
// Method:      vBCSMain                                                //
// Description: Get data and control the balance of cycle-car for each  //
//              10ms by applying a PID control                          //
// Parameters:  n/a                                                     //
// Returns:     n/a                                                     //
//**********************************************************************//
void vBCSMain(void)
{

	vCommunicationStartBroadcast(1000);

	while (1)
	{
		if(timFlags.ui10ms == 1)
		{
			// Get data from motor
			Get_Encoder_Estimates(Axes[0]);
			Get_IQ(Axes[0]);
			Get_Temperature(Axes[0]);

			// Get data from IMU
			cSCHA63TRead(&SCHA63TData);
			fAccelerationMeasured = SCHA63TData.data_accl[0] + 0.00;
			fAccelerationAngle    = radToDeg(asin(fAccelerationMeasured/gravity));

			// Estimate motor acceleration and apply PID control
			fMotorAcceleration = (degToRad(motorTurnsToAngle(Axes[0].AXIS_Encoder_Vel)) - fLastMotorSpeed) / (UPDATE_RATE);
			fLastMotorSpeed = degToRad(motorTurnsToAngle(Axes[0].AXIS_Encoder_Vel));
			//fAccelerationMeasured -= filter(IMU_HEIGHT*fMotorAcceleration);

			vActuadorRun();

			timFlags.ui10ms = 0;
		}
	}
}


//**********************************************************************//
// Method:      HAL_TIM_PeriodElapsedCallback                           //
// Description: Callback function of period elapsed in STM32 timers     //
// Parameters:  TIM_HandleTypeDef *: pointer to timer that trigger the  //
//                                   interruption                       //
// Returns:     n/a                                                     //
//**********************************************************************//
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// htim7 is triggered with an rate of 10ms
	if(htim == &htim7)
	{
		if(timFlags.ui10ms)
		{
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
		}

		timFlags.ui10ms = 1;
		timFlags.uiCounter++;

		if(timFlags.uiCounter % 10 == 0)
			timFlags.ui100ms = 1;
		if(timFlags.uiCounter % 100 == 0)
		{
			timFlags.ui1000ms = 1;
			timFlags.uiCounter = 0;
		}
	}
	else if(htim == &htim6)
		vCommunicationSendBroadcast();
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
			ODrive_RX_CallBack(Axes, &RxHeader, RxData);
	}
}


