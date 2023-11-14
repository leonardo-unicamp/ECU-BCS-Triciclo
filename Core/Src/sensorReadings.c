//**********************************************************************//
// File:        sensorReadings.c                                        //
// Description: Source file that contains the functions and methods     //
//              implementations for handling with sensor readings task  //
// Author:      Leonardo Rossi Leao                                     //
// Reviewer:    Rodrigo Moreira Bacurau                                 //
// Last update: 2023-11-13                                              //
//**********************************************************************//

#include "sensorReadings.h"

extern Axis xAxes[2];
SCHA63TOutput SCHA63TData;

float fLastMotorSpeed;
float fMotorAcceleration;
float fActuadorTorque;
float fAccelerationMeasured;
float fAccelerationAngle;

sensorReadings_t xSensorReadings;

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
			// Get data from motor
			Get_Encoder_Estimates(xAxes[0]);
			Get_IQ(xAxes[0]);
			Get_Temperature(xAxes[0]);

			// Get data from IMU
			cSCHA63TRead(&SCHA63TData);
			fAccelerationMeasured = SCHA63TData.data_accl[0] + 0.00;
			fAccelerationAngle    = radToDeg(asin(fAccelerationMeasured/gravity));

			// Estimate motor acceleration and apply PID control
			fMotorAcceleration = (degToRad(motorTurnsToAngle(xAxes[0].AXIS_Encoder_Vel)) - fLastMotorSpeed) / (UPDATE_RATE);
			fLastMotorSpeed = degToRad(motorTurnsToAngle(xAxes[0].AXIS_Encoder_Vel));
			//fAccelerationMeasured -= filter(IMU_HEIGHT*fMotorAcceleration);

			xSensorReadings.fIMUAcceleration = fAccelerationMeasured;
			xSensorReadings.fIMUAccelerationAngle = fAccelerationAngle;
			xSensorReadings.fSuspensionMotorAcceleration = fMotorAcceleration;
		}
}
