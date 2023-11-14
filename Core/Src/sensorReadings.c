//**********************************************************************//
// File:        sensorReadings.c                                        //
// Description: Source file that contains the functions and methods     //
//              implementations for handling with sensor readings task  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-11-13                                              //
//**********************************************************************//

#include "sensorReadings.h"
#include "BCS_main.h"

timerFlags_t timFlags;
Axis Axes[MAX_AXES];
SCHA63TOutput SCHA63TData;

float fLastMotorSpeed;
float fMotorAcceleration;
float fActuadorTorque;
float fAccelerationMeasured;
float fAccelerationAngle;

void vSensorReadingsInit(void){

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
}

//**********************************************************************//
// Method:      vSensorReadingsStart                                    //
// Description: Entrypoint for sensor readings task. Use libraries to   //
//              acquisite sensor data and store into a data struct      //
// Input:       none                                                    //
// Output:      none                                                    //
//**********************************************************************//
void vSensorReadingsStart()
{
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
